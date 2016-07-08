#pragma once

using namespace System;

namespace SAPStudio {
	namespace AvsFilterNet {
		public ref class VarNotFoundException :
			public ApplicationException {
		public:
			VarNotFoundException(String^ varName) : ApplicationException(String::Format("Variable \"{0}\" not found.", varName)) { }
			VarNotFoundException(void) : ApplicationException("Variable not found.") { }
		};
	}
}