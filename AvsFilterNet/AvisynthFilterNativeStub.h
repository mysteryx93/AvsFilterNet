#include <msclr/gcroot.h>

namespace SAPStudio {
	namespace AvsFilterNet {
		class AvisynthFilterNativeStub sealed : public IClip {
        protected:
            PClip child;
            NativeVideoInfo vi;
		private:
			msclr::gcroot<AvisynthFilter ^ > _impl;
		public:
            AvisynthFilterNativeStub(PClip _child, AvisynthFilter^ impl);
			AvisynthFilterNativeStub(AvisynthFilter^ impl);
			~AvisynthFilterNativeStub();

            PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env);
            void __stdcall GetAudio(void* buf, __int64 start, __int64 count, IScriptEnvironment* env);
            const NativeVideoInfo& __stdcall GetVideoInfo();
            bool __stdcall GetParity(int n);
            int __stdcall SetCacheHints(int cachehints, int frame_range);

			void SetVideoInfo(const NativeVideoInfo& vi);
			void SetChild(const PClip c);
		};
	};
};