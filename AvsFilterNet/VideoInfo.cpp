#include "stdafx.h"

namespace AvsFilterNet {
	// useful functions of the above
	bool VideoInfo::HasVideo() { return (width != 0); }
	bool VideoInfo::HasAudio() { return (audio_samples_per_second != 0); }
	bool VideoInfo::IsRGB() { return !!(int)(pixel_type&ColorSpaces::CS_BGR); }
	bool VideoInfo::IsRGB24() { return (pixel_type&ColorSpaces::CS_BGR24) == ColorSpaces::CS_BGR24; } // Clear out additional properties
	bool VideoInfo::IsRGB32() { return (pixel_type & ColorSpaces::CS_BGR32) == ColorSpaces::CS_BGR32; }
	bool VideoInfo::IsYUV() { return !!(int)(pixel_type&ColorSpaces::CS_YUV); }
	bool VideoInfo::IsYUY2() { return (pixel_type & ColorSpaces::CS_YUY2) == ColorSpaces::CS_YUY2; }
	bool VideoInfo::IsYV12() { return ((pixel_type & ColorSpaces::CS_YV12) == ColorSpaces::CS_YV12) || ((pixel_type & ColorSpaces::CS_I420) == ColorSpaces::CS_I420); }
	bool VideoInfo::IsColorSpace(ColorSpaces c_space) { return ((pixel_type & c_space) == c_space); }
	bool VideoInfo::Is(ColorSpaces property) { return ((pixel_type & property) == property); }
	bool VideoInfo::IsPlanar() { return !!(int)(pixel_type & ColorSpaces::CS_PLANAR); }
	bool VideoInfo::IsFieldBased() { return !!(int)(image_type & FrameType::IT_FIELDBASED); }
	bool VideoInfo::IsParityKnown() { return (((int)(image_type & FrameType::IT_FIELDBASED)) && ((int)(image_type & (FrameType::IT_BFF | FrameType::IT_TFF)))); }
	bool VideoInfo::IsBFF() { return !!(int)(image_type & FrameType::IT_BFF); }
	bool VideoInfo::IsTFF() { return !!(int)(image_type & FrameType::IT_TFF); }

	bool VideoInfo::IsVPlaneFirst() { return ((pixel_type & ColorSpaces::CS_YV12) == ColorSpaces::CS_YV12); }  // Don't use this
	int VideoInfo::BytesFromPixels(int pixels) { return pixels * (BitsPerPixel() >> 3); }   // Will not work on planar images, but will return only luma planes
	int VideoInfo::RowSize() { return BytesFromPixels(width); }  // Also only returns first plane on planar images
	int VideoInfo::BMPSize() { if (IsPlanar()) { int p = height * ((RowSize() + 3) & ~3); p += p >> 1; return p; } return height * ((RowSize() + 3) & ~3); }
	__int64 VideoInfo::AudioSamplesFromFrames(__int64 frames) { return (fps_numerator && HasVideo()) ? ((__int64)(frames)* audio_samples_per_second * fps_denominator / fps_numerator) : 0; }
	int VideoInfo::FramesFromAudioSamples(__int64 samples) { return (fps_denominator && HasAudio()) ? (int)((samples * (__int64)fps_numerator) / ((__int64)fps_denominator * (__int64)audio_samples_per_second)) : 0; }
	__int64 VideoInfo::AudioSamplesFromBytes(__int64 bytes) { return HasAudio() ? bytes / BytesPerAudioSample() : 0; }
	__int64 VideoInfo::BytesFromAudioSamples(__int64 samples) { return samples * BytesPerAudioSample(); }
	int VideoInfo::AudioChannels() { return HasAudio() ? nchannels : 0; }
	//int VideoInfo::SampleType() { return sample_type;}
	bool VideoInfo::IsSampleType(AudioSampleType testtype) { return !!(int)(sample_type&testtype); }
	int VideoInfo::SamplesPerSecond() { return audio_samples_per_second; }
	int VideoInfo::BytesPerAudioSample() { return nchannels*BytesPerChannelSample(); }
	void VideoInfo::SetFieldBased(bool isfieldbased) { if (isfieldbased) image_type = image_type | FrameType::IT_FIELDBASED; else  image_type = image_type&~FrameType::IT_FIELDBASED; }
	void VideoInfo::Set(FrameType property) { image_type = image_type | property; }
	void VideoInfo::Clear(FrameType property) { image_type = image_type&~property; }

	int VideoInfo::BitsPerPixel() {
		switch (pixel_type) {
		case ColorSpaces::CS_BGR24:
			return 24;
		case ColorSpaces::CS_BGR32:
			return 32;
		case ColorSpaces::CS_YUY2:
			return 16;
		case ColorSpaces::CS_YV12:
		case ColorSpaces::CS_I420:
			return 12;
		default:
			return 0;
		}
	}

	int VideoInfo::BytesPerChannelSample() {
		switch (sample_type) {
		case AudioSampleType::SAMPLE_INT8:
			return sizeof(signed char);
		case AudioSampleType::SAMPLE_INT16:
			return sizeof(signed short);
		case AudioSampleType::SAMPLE_INT24:
			return 3;
		case AudioSampleType::SAMPLE_INT32:
			return sizeof(signed int);
		case AudioSampleType::SAMPLE_FLOAT:
			return sizeof(SFLOAT);
		default:
			_ASSERTE("Sample type not recognized!");
			return 0;
		}
	}

	// useful mutator
	void VideoInfo::SetFPS(unsigned numerator, unsigned denominator) {
		if ((numerator == 0) || (denominator == 0)) {
			fps_numerator = 0;
			fps_denominator = 1;
		}
		else {
			unsigned x = numerator, y = denominator;
			while (y) {   // find gcd
				unsigned t = x%y; x = y; y = t;
			}
			fps_numerator = numerator / x;
			fps_denominator = denominator / x;
		}
	}

	// Range protected multiply-divide of FPS
	void VideoInfo::MulDivFPS(unsigned multiplier, unsigned divisor) {
		unsigned __int64 numerator = UInt32x32To64(fps_numerator, multiplier);
		unsigned __int64 denominator = UInt32x32To64(fps_denominator, divisor);

		unsigned __int64 x = numerator, y = denominator;
		while (y) {   // find gcd
			unsigned __int64 t = x%y; x = y; y = t;
		}
		numerator /= x; // normalize
		denominator /= x;

		unsigned __int64 temp = numerator | denominator; // Just looking top bit
		unsigned u = 0;
		while (temp & 0xffffffff80000000) { // or perhaps > 16777216*2
			temp = Int64ShrlMod32(temp, 1);
			u++;
		}
		if (u) { // Scale to fit
			unsigned round = 1 << (u - 1);
			SetFPS((unsigned)Int64ShrlMod32(numerator + round, u),
				(unsigned)Int64ShrlMod32(denominator + round, u));
		}
		else {
			fps_numerator = (unsigned)numerator;
			fps_denominator = (unsigned)denominator;
		}
	}

	// Test for same colorspace
	bool VideoInfo::IsSameColorspace(VideoInfo% vi) {
		if (vi.pixel_type == pixel_type) return TRUE;
		if (IsYV12() && vi.IsYV12()) return TRUE;
		return FALSE;
	}

	VideoInfo VideoInfo::FromNative(const NativeVideoInfo* nvi) {
		return (VideoInfo)Marshal::PtrToStructure(IntPtr((void*)nvi), VideoInfo::typeid);
	}
}