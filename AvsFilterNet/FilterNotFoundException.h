#pragma once

using namespace System;

namespace AvsFilterNet {

	public ref class FilterNotFoundException :
		public ApplicationException {
	public:
		FilterNotFoundException(String^ filterName) : ApplicationException(String::Format("Filter \"{0}\" not found.", filterName)) { }
		FilterNotFoundException() : ApplicationException("Filter not found.") { }
	};
}