using namespace System;
using namespace System::Runtime::InteropServices;

namespace SAPStudio {
	namespace AvsFilterNet {
		private ref class NativeString sealed {
		private:
			IntPtr ip;
			const char* str;
			void CleanUp();

		public:
			NativeString(String^ s);
			~NativeString();
			const char* GetPointer();
		};
	};
};