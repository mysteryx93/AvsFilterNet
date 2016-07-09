//Enumerations from avisynth.h
namespace AvsFilterNet {
	public enum class PlanarChromaAlignmentMode {
		PlanarChromaAlignmentOff,
		PlanarChromaAlignmentOn,
		PlanarChromaAlignmentTest
	};

	public enum class AudioSampleType {
		SAMPLE_INT8 = 1 << 0,
		SAMPLE_INT16 = 1 << 1,
		SAMPLE_INT24 = 1 << 2,    // Int24 is a very stupid thing to code, but it's supported by some hardware.
		SAMPLE_INT32 = 1 << 3,
		SAMPLE_FLOAT = 1 << 4
	};

	public enum class YUVPlanes {
		PLANAR_Y = 1 << 0,
		PLANAR_U = 1 << 1,
		PLANAR_V = 1 << 2,
		PLANAR_ALIGNED = 1 << 3,
		PLANAR_Y_ALIGNED = PLANAR_Y | PLANAR_ALIGNED,
		PLANAR_U_ALIGNED = PLANAR_U | PLANAR_ALIGNED,
		PLANAR_V_ALIGNED = PLANAR_V | PLANAR_ALIGNED,
	};

	// Colorspace properties.
	public enum class ColorSpaces {
		CS_BGR = 1 << 28,
		CS_YUV = 1 << 29,
		CS_INTERLEAVED = 1 << 30,
		CS_PLANAR = 1 << 31,
		CS_UNKNOWN = 0,
		CS_BGR24 = 1 << 0 | CS_BGR | CS_INTERLEAVED,
		CS_BGR32 = 1 << 1 | CS_BGR | CS_INTERLEAVED,
		CS_YUY2 = 1 << 2 | CS_YUV | CS_INTERLEAVED,
		CS_YV12 = 1 << 3 | CS_YUV | CS_PLANAR,  // y-v-u, 4:2:0 planar
		CS_I420 = 1 << 4 | CS_YUV | CS_PLANAR,  // y-u-v, 4:2:0 planar
		CS_IYUV = 1 << 4 | CS_YUV | CS_PLANAR,  // same as above
	};

	public enum class FrameType {
		IT_BFF = 1 << 0,
		IT_TFF = 1 << 1,
		IT_FIELDBASED = 1 << 2
	};

	public enum class CacheType {
		CACHE_NOTHING = 0,
		CACHE_RANGE = 1,
		CACHE_ALL = 2,
		CACHE_AUDIO = 3,
		CACHE_AUDIO_NONE = 4,
		CACHE_AUDIO_AUTO = 5
	};

	[System::Flags]
	public enum class CPUFlags {
		/* slowest CPU to support extension */
		CPUF_FORCE = 0x01,   //  N/A
		CPUF_FPU = 0x02,   //  386/486DX
		CPUF_MMX = 0x04,   //  P55C, K6, PII
		CPUF_INTEGER_SSE = 0x08,   //  PIII, Athlon
		CPUF_SSE = 0x10,   //  PIII, Athlon XP/MP
		CPUF_SSE2 = 0x20,   //  PIV, Hammer
		CPUF_3DNOW = 0x40,   //  K6-2
		CPUF_3DNOW_EXT = 0x80,   //  Athlon
		CPUF_X86_64 = 0xA0,   //  Hammer (note: equiv. to 3DNow + SSE2, which
		//          only Hammer will have anyway)
		CPUF_SSE3 = 0x100,   //  PIV+, Hammer
	};

	public enum class MtMode {
		UNKNOWN = 0,
		NICE_FILTER = 1,
		MULTI_INSTANCE = 2,
		SERIALIZED = 3,
	};
}