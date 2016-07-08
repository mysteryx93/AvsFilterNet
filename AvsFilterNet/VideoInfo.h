using namespace System::Runtime::InteropServices;

typedef ::VideoInfo NativeVideoInfo;

namespace SAPStudio {
	namespace AvsFilterNet {
		[StructLayout(LayoutKind::Sequential)]
		public value struct VideoInfo {
		public:
			int width;
			int height;    // width=0 means no video
			unsigned fps_numerator;
			unsigned fps_denominator;
			int num_frames;
			// This is more extensible than previous versions. More properties can be added seeminglesly.

			ColorSpaces pixel_type;                // changed to int as of 2.5

			int audio_samples_per_second;   // 0 means no audio
			AudioSampleType sample_type;                // as of 2.5
			__int64 num_audio_samples;      // changed as of 2.5
			int nchannels;                  // as of 2.5

			// Imagetype properties

			FrameType image_type;
				// useful functions of the above
				bool HasVideo();
			bool HasAudio();
			bool IsRGB();
			bool IsRGB24(); // Clear out additional properties
			bool IsRGB32();
			bool IsYUV();
			bool IsYUY2();
			bool IsYV12();
			bool IsColorSpace(ColorSpaces c_space);
			bool Is(ColorSpaces property);
			bool IsPlanar();
			bool IsFieldBased();
			bool IsParityKnown();
			bool IsBFF();
			bool IsTFF();

			bool IsVPlaneFirst();  // Don't use this
			int BytesFromPixels(int pixels);   // Will not work on planar images, but will return only luma planes
			int RowSize();  // Also only returns first plane on planar images
			int BMPSize();
			__int64 AudioSamplesFromFrames(__int64 frames);
			int FramesFromAudioSamples(__int64 samples);
			__int64 AudioSamplesFromBytes(__int64 bytes);
			__int64 BytesFromAudioSamples(__int64 samples);
			int AudioChannels();
			//int SampleType();
			bool IsSampleType(AudioSampleType testtype);
			int SamplesPerSecond();
			int BytesPerAudioSample();
			void SetFieldBased(bool isfieldbased);
			void Set(FrameType property);
			void Clear(FrameType property);

			int BitsPerPixel();

			int BytesPerChannelSample();

			// useful mutator
			void SetFPS(unsigned numerator, unsigned denominator);

			// Range protected multiply-divide of FPS
			void MulDivFPS(unsigned multiplier, unsigned divisor);

			// Test for same colorspace
			bool IsSameColorspace( VideoInfo% vi);
		
		internal:
			static VideoInfo FromNative(const NativeVideoInfo* nvi);
		};
	}
}