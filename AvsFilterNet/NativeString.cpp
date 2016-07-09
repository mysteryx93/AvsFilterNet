#include "stdafx.h"

namespace SAPStudio {
	namespace AvsFilterNet {
		NativeString::NativeString(String^ s) {
			ip = Marshal::StringToHGlobalAnsi(s);
			str = static_cast<const char*>(ip.ToPointer());
		}

		NativeString::~NativeString() {
			CleanUp();
		}

		const char* NativeString::GetPointer() {
			return str;
		}

		void NativeString::CleanUp() {
			if (str) {
				str = NULL;
				Marshal::FreeHGlobal(ip);
				ip = IntPtr::Zero;
			}
		}
	};
};