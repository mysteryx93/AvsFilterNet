#include "stdafx.h"

namespace SAPStudio {
	namespace AvsFilterNet {
		AvisynthFilterNativeStub::AvisynthFilterNativeStub(PClip _child, AvisynthFilter^ impl) : child(_child) {
			_impl = impl;
		}

		AvisynthFilterNativeStub::AvisynthFilterNativeStub(AvisynthFilter^ impl) {
			memset(&vi, 0, sizeof(vi));
			_impl = impl;
		}

		AvisynthFilterNativeStub::~AvisynthFilterNativeStub(){
			delete _impl;
			_impl=nullptr;
		}

		PVideoFrame __stdcall AvisynthFilterNativeStub::GetFrame(int n, IScriptEnvironment* env){
			PVideoFrame ret;
			char * errMsg=NULL;
			try
			{
				ret = _impl->GetFrame(n,env);
			}
			catch (AvisynthError err) 
			{
				// To prevent problems, we always duplicate error messages
				errMsg=(_strdup(err.msg));
			}
			catch (AvisynthException^ ex){
				errMsg=((char *)Marshal::StringToHGlobalAnsi(ex->Message).ToPointer());
			}
			catch (SEHException^ ex)
			{
                (void)ex;
				errMsg=("SEHException");
			}
			catch (Exception^ ex)
			{
				errMsg=((char *)Marshal::StringToHGlobalAnsi(_impl->GetType()->Name + ": " + ex->ToString()).ToPointer());
			}
			if (errMsg) env->ThrowError(errMsg);
			if (ret) return ret;
			return child->GetFrame(n,env);
		}

		void __stdcall AvisynthFilterNativeStub::GetAudio(void* buf, __int64 start, __int64 count, IScriptEnvironment* env){
			try
			{
				_impl->GetAudio(buf,start,count,env);
			}
			catch (AvisynthError err) {
				// To prevent problems, we alway duplicate error messages
				env->ThrowError(_strdup(err.msg));
			}
			catch (AvisynthException^ ex){
				env->ThrowError((char *)Marshal::StringToHGlobalAnsi(ex->Message).ToPointer());
			}
			catch (Exception^ ex)
			{
				env->ThrowError((char *)Marshal::StringToHGlobalAnsi(_impl->GetType()->Name + ": " + ex->ToString()).ToPointer());
			}
			catch (...) 
			{
				env->ThrowError("An unknown error while getting audio samples.");
			}
		}

		void AvisynthFilterNativeStub::SetVideoInfo(const NativeVideoInfo& nvi) {
			vi=nvi;
		}

		void AvisynthFilterNativeStub::SetChild(const PClip c){
			child=c;
			vi=c->GetVideoInfo();
		}

		const NativeVideoInfo& __stdcall AvisynthFilterNativeStub::GetVideoInfo(){
			_impl->InitComplete();
			return vi;
		}

		bool __stdcall AvisynthFilterNativeStub::GetParity(int n){
			return _impl->GetParity(n);
		}

		int __stdcall AvisynthFilterNativeStub::SetCacheHints(int cachehints,int frame_range){
			return _impl->SetCacheHints((CacheType)cachehints,frame_range);
		}
	}
}