#include "StdAfx.h"

namespace SAPStudio {
	namespace AvsFilterNet {
		//for calling AddRef and Release, is it nesessary?
		Clip::Clip(PNativeClip clip) {
			_pclip = (new PNativeClip(clip));
			_clip = (NativeClip*)(void*)*_pclip;
		}
		Clip::~Clip() {
			CleanUp();
		}
		Clip::!Clip() {
#if DEBUG
			System::Diagnostics::Debug::WriteLine("Clip::!Clip start");
#endif
			CleanUp();
#if DEBUG
			System::Diagnostics::Debug::WriteLine("Clip::!Clip end");
#endif
		}
		void Clip::CleanUp()
		{
			_clip = NULL;
			delete _pclip;
			_pclip = NULL;
		}

		PNativeClip Clip::GetNative() {
			return PNativeClip(_clip);
		}

		int Clip::GetVersion() {
			return (_clip)->GetVersion();
		}

		VideoFrame^ Clip::GetFrame(int n, ScriptEnvironment^ env) {
			return gcnew VideoFrame((_clip)->GetFrame(n, env->GetNative()));
		}

		bool Clip::GetParity(int n) {
			return (_clip)->GetParity(n);
		}

		void Clip::GetAudio(IntPtr buf, Int64 start, Int64 count, ScriptEnvironment^ env) {
			(_clip)->GetAudio(buf.ToPointer(), start, count, env->GetNative());
		}

		int Clip::SetCacheHints(CacheType cachehints, int frame_range) {
			return (_clip)->SetCacheHints((int)cachehints, frame_range);
		}

		VideoInfo Clip::GetVideoInfo() {
			return VideoInfo::FromNative(&((_clip)->GetVideoInfo()));
		}
	}
}