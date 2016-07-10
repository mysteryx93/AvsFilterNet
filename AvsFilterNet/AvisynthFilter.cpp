#include "stdafx.h"

namespace AvsFilterNet {
	AvisynthFilter::AvisynthFilter() {
		_stub = new AvisynthFilterNativeStub(this);
		_initialized = false;
	}

	AvisynthFilter::~AvisynthFilter() {
		_stub = nullptr;
		if (_child) {
			delete _child;
			_child = nullptr;
		}
	}

	void AvisynthFilter::Initialize(AVSValue^ args, ScriptEnvironment^ env) {
	}

	AVSValue^ AvisynthFilter::ExecuteBefore(AVSValue^ args, bool% cancelLoad, ScriptEnvironment^ env) {
		return gcnew AVSValue(args[0]->AsClip());
	}

	AVSValue^ AvisynthFilter::ExecuteAfter(AVSValue^ clip, ScriptEnvironment^ env) {
		return gcnew AVSValue(clip->AsClip());
	}

	Clip^ AvisynthFilter::Child::get() {
		return _child;
	}

	void AvisynthFilter::GetAudio(IntPtr buf, __int64 start, __int64 count, ScriptEnvironment^ env) {
		if (_child)
			_child->GetAudio(buf, start, count, env);
	}

	bool AvisynthFilter::GetParity(int n) {
		return _child ? _child->GetParity(n) : false;
	}

	int AvisynthFilter::SetCacheHints(CacheType cachehints, int frame_range) {
		return _child ? _child->SetCacheHints(cachehints, frame_range) : 0;
	}

	VideoInfo AvisynthFilter::GetVideoInfo() {
		return _vi;
	}

	void AvisynthFilter::SetVideoInfo(VideoInfo% vi) {
		if (_initialized) throw gcnew InvalidOperationException("You can only set VideoInfo during initialization.");
		_vi = vi;
		pin_ptr<VideoInfo> pvi = &vi;
		_stub->SetVideoInfo(*(NativeVideoInfo*)(void*)pvi);
	}

	void AvisynthFilter::SetChild(Clip^ child) {
		if (_initialized) throw gcnew InvalidOperationException("You can only set child during initialization.");
		if (_child == child) return;
		if (_child) delete _child;
		_child = child;
		if (child) {
			_vi = child->GetVideoInfo();
			_stub->SetChild(child->GetNative());
		}
		else {
			_stub = new AvisynthFilterNativeStub(this);
		}
		_initialized = false;
	}

	VideoFrame^ AvisynthFilter::NewVideoFrame(ScriptEnvironment^ env) {
		return NewVideoFrame(FRAME_ALIGN, env);
	}

	VideoFrame^ AvisynthFilter::NewVideoFrame(int align, ScriptEnvironment^ env) {
		return env->NewVideoFrame(_vi, align);
	}

	PVideoFrame AvisynthFilter::GetFrame(int n, IScriptEnvironment* env) {
		VideoFrame^ ret = GetFrame(n, gcnew ScriptEnvironment(env));
		if (ret) {
			PVideoFrame pvf = ret->GetNative();
			delete ret;
			return pvf;
		}
		return NULL;
	}

	void AvisynthFilter::GetAudio(void* buf, __int64 start, __int64 count, IScriptEnvironment* env) {
		GetAudio(IntPtr(buf), start, count, gcnew AvsFilterNet::ScriptEnvironment(env));
	}

	AvisynthFilterNativeStub* AvisynthFilter::GetNativeStub() {
		return _stub;
	}

	void AvisynthFilter::InitComplete() {
		_initialized = true;
	}
}