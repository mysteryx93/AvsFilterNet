#pragma once

using namespace System;

namespace SAPStudio {
	namespace AvsFilterNet {

		public ref class AvisynthException :
			public ApplicationException
			{
			public:

				AvisynthException(String^ message) : ApplicationException(message)
				{
				}
				AvisynthException() : ApplicationException("Avisynth error.")
				{
				}
			};
	}
}