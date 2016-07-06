#include "stdafx.h"


namespace SAPStudio {
	namespace AvsFilterNet {

		NativeString::NativeString(String^ s) {
			ip = Marshal::StringToHGlobalAnsi(s);
			str = static_cast<const char*>(ip.ToPointer());
		};
		NativeString::~NativeString() {
			CleanUp();
		};
		const char* NativeString::GetPointer(){
			return str;
		};

		//NativeString::!NativeString(){
		//	System::Diagnostics::Debug::WriteLine("NativeString::!NativeString start");
		//	CleanUp();
		//	System::Diagnostics::Debug::WriteLine("NativeString::!NativeString end");
		//};
		void NativeString::CleanUp()
		{
			if (str) {
				str = NULL;
				Marshal::FreeHGlobal(ip);
				ip = IntPtr::Zero;
			};
		}
	};
};