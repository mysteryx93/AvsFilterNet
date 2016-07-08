#include "stdafx.h"

namespace SAPStudio {
    namespace AvsFilterNet {
        AvisynthFilter::AvisynthFilter() {
            //if (args[0]->IsClip()) {
            //    _child = args[0]->AsClip();
            //    _stub = new AvisynthFilterNativeStub(_child->GetNative(), this);
            //    _vi = VideoInfo::FromNative(&(_stub->GetVideoInfo()));
            //}
            //else {
                _stub = new AvisynthFilterNativeStub(this);
            //}
            _initialized = false;
        }

        AvisynthFilter::~AvisynthFilter() {
            _stub = nullptr;
            if (_child) {
                delete _child;
                _child = nullptr;
            }
        }

        AVSValue^ AvisynthFilter::Initialize(AVSValue^ args, ScriptEnvironment^ env) { return args; }
        AVSValue^ AvisynthFilter::Finalize(AVSValue^ clip, ScriptEnvironment^ env) { return clip; }

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
            if (child) {
                _vi = child->GetVideoInfo();
                _stub->SetChild(child->GetNative());
            }
            else {
                _stub = new AvisynthFilterNativeStub(this);
            }
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
            GetAudio(IntPtr(buf), start, count, gcnew SAPStudio::AvsFilterNet::ScriptEnvironment(env));
        }

        AvisynthFilterNativeStub* AvisynthFilter::GetNativeStub() {
            return _stub;
        }

        void AvisynthFilter::InitComplete() {
            _initialized = true;
        }
    };
};