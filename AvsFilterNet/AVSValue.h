#ifndef AVSVALUE_H
#define AVSVALUE_H

typedef AVSValue NativeAVSValue;

using namespace System;

namespace AvsFilterNet {
	public ref class AVSValue sealed {
	private:
		NativeAVSValue* _value;
		NativeString^ _nts;
		NativeAVSValue* _arrptr;
		void CleanUp();
	public:
		AVSValue();
		AVSValue(AvsFilterNet::Clip^ c);
		AVSValue(bool b);
		AVSValue(int i);
		AVSValue(float f);
		AVSValue(double f);
		AVSValue(String^ s);
		AVSValue(... array<AVSValue^>^ a);

		//AVSValue(const AVSValue& v) { Assign(&v, true); }

		~AVSValue();
		!AVSValue();

		bool Defined();
		bool IsClip();
		bool IsBool();
		bool IsInt();
		bool IsFloat();
		bool IsString();
		bool IsArray();

		Clip^ AsClip();
		bool AsBool();
		int AsInt();
		String^ AsString();
		double AsFloat();
		bool AsBool(bool def);
		int AsInt(int def);
		double AsFloat(double def);
		String^ AsString(String^ def);

		int ArraySize();
		property AVSValue^ AVSValue::default[int]{
			AVSValue^ get(int index);
		}
	internal:
		AVSValue(NativeAVSValue value);
		NativeAVSValue GetNative();
		void CollectorDispose();
	};
};

#endif