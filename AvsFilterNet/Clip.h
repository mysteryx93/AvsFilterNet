

#ifndef CLIP_H
#define CLIP_H


typedef PClip PNativeClip;
typedef IClip NativeClip;
using namespace System;
namespace SAPStudio {
	namespace AvsFilterNet {
		ref class ScriptEnvironment;
		public ref class Clip sealed {
		private:
			NativeClip* _clip;
			PNativeClip* _pclip;
			void CleanUp();
		internal:
			Clip(PNativeClip clip);
			~Clip();
			PNativeClip GetNative();
		protected:
			!Clip();

		public:
			int GetVersion();
			VideoFrame^ GetFrame(int n, ScriptEnvironment ^env);
			bool GetParity(int n);
			void GetAudio(IntPtr buf, Int64 start, Int64 count, ScriptEnvironment ^env);
			int SetCacheHints(CacheType cachehints, int frame_range);
			VideoInfo GetVideoInfo();
		};
	};
};



#endif