#include "stdafx.h"

namespace SAPStudio {
    namespace AvsFilterNet {
        AvisynthFilter::AvisynthFilter(AVSValue^ args, ScriptEnvironment^ env) {
            _env = env;
            _nenv = env->GetNative();
            if (args[0]->IsClip()) {
                _child = args[0]->AsClip();
                _stub = new AvisynthFilterNativeStub(_child->GetNative(), this);
                _vi = VideoInfo::FromNative(&(_stub->GetVideoInfo()));
            }
            else {
                _stub = new AvisynthFilterNativeStub(this);
            }
            _initialized = false;
        }

        AvisynthFilter::~AvisynthFilter() {
            _env = nullptr;
            _nenv = nullptr;
            _stub = nullptr;
            if (_child) {
                delete _child;
                _child = nullptr;
            }
        }

        AVSValue^ AvisynthFilter::Closing(ScriptEnvironment^ env) { return gcnew AVSValue(_stub); }

        Clip^ AvisynthFilter::Child::get() {
            return _child;
        }

        //VideoFrame^ AvisynthFilter::GetFrame(int n, ScriptEnvironment^ env)	{
        //	return Child->GetFrame(n,env); //fetch frame from child
        //}

        void AvisynthFilter::GetAudio(IntPtr buf, __int64 start, __int64 count, ScriptEnvironment^ env) {
            _child->GetAudio(buf, start, count, env);
        }

        bool AvisynthFilter::GetParity(int n) {
            return _child->GetParity(n);
        }
        int AvisynthFilter::SetCacheHints(CacheType cachehints, int frame_range) {
            return _child->SetCacheHints(cachehints, frame_range);
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
            _vi = child->GetVideoInfo();
            _stub->SetChild(child->GetNative());
        }

        VideoFrame^ AvisynthFilter::NewVideoFrame() {
            return NewVideoFrame(FRAME_ALIGN);
        }
        VideoFrame^ AvisynthFilter::NewVideoFrame(int align) {
            pin_ptr<VideoInfo> vi = &_vi;
            return gcnew VideoFrame(_nenv->NewVideoFrame(*(NativeVideoInfo*)(void*)vi, align));
        }

        void AvisynthFilter::CheckEnvPointer(IScriptEnvironment *env)
        {
            if (_nenv != env) {
                _nenv = env;
                _env = gcnew ScriptEnvironment(env);
            }
        }
        PVideoFrame AvisynthFilter::GetFrame(int n, IScriptEnvironment* env) {
            CheckEnvPointer(env);
            VideoFrame^ ret = GetFrame(n, _env);
            if (ret) {
                PVideoFrame pvf = ret->GetNative();
                delete ret;
                return pvf;
            }
            return NULL;
        }
        void AvisynthFilter::GetAudio(void* buf, __int64 start, __int64 count, IScriptEnvironment* env) {
            CheckEnvPointer(env);
            GetAudio(IntPtr(buf), start, count, _env);
        }

        AvisynthFilterNativeStub* AvisynthFilter::GetNativeStub() {
            return _stub;
        }

        void AvisynthFilter::InitComplete() {
            _initialized = true;
        }
    };
};