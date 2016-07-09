using namespace System;

namespace AvsFilterNet {
	[AttributeUsage(AttributeTargets::Assembly, AllowMultiple = true)]
	public ref class AvisynthFilterClassAttribute sealed : Attribute {
	public:
		Type^ FilterType;
		String^ FilterName;
		String^ Arguments;
		MtMode MultiThreadingMode;

		/// <summary>
		///     The constructor of AvisynthFilterClassAttribute.
		/// </summary>
		/// <param name="type">Your plugin type. It must be derived from AvisynthFilter.</param>
		/// <param name="filterName">Plugin name. This is the name used in AVS scripts.</param>
		/// <param name="arguments">Plugin arguments. It is a string that defines the types and optional names of the arguments for you filter.<br/>
		///                                         c - Video Clip<br/>
		///                                         i - Integer number<br/>
		///                                         f - Float number<br/>
		///                                         s - String<br/>
		///                                         b - boolean<br/>
		///                                 The word inside the [ ] lets you used named parameters in your script<br/>
		///                                 e.g last=SimpleSample(last,windowclip,size=100).<br/>
		///                                 but last=SimpleSample(last,windowclip, 100) will also work <br/>
		/// Any type can be followed with a '*' or '+' to indicate "zero or more" or "one or more" respectively. <br/>
		/// In this case all the matching arguments will be gathered into a sub-array.<br/>
		/// For example, if your type string is "is+f", then the integer argument will be args[0], <br/>
		/// the string arguments will be args[1][0], args[1][1], etc. , and the float argument will be args[2].<br/>
		/// '.' matches a single argument of any type. To match multiple arguments of any type, use ".*" or ".+". </param>

		AvisynthFilterClassAttribute(Type^ type, String^ filterName, String^ arguments) : AvisynthFilterClassAttribute(type, filterName, arguments, MtMode::UNKNOWN) {
		}

		AvisynthFilterClassAttribute(Type^ type, String^ filterName, String^ arguments, MtMode multiThreadingMode) {
			this->FilterType = type;
			this->FilterName = filterName;
			this->Arguments = arguments;
			this->MultiThreadingMode = multiThreadingMode;
		}
	};
};
