#include "stdafx.h"

namespace AvsFilterNet {
	AVSValue::AVSValue() { _value = new NativeAVSValue(); AVSValueCollector::OnAVSValueCreate(this); }
	AVSValue::AVSValue(Clip ^ c) { _value = new NativeAVSValue(c->GetNative()); AVSValueCollector::OnAVSValueCreate(this); }
	AVSValue::AVSValue(bool b) { _value = new NativeAVSValue(b); AVSValueCollector::OnAVSValueCreate(this); }
	AVSValue::AVSValue(int i) { _value = new NativeAVSValue(i); AVSValueCollector::OnAVSValueCreate(this); }
	AVSValue::AVSValue(float f) { _value = new NativeAVSValue(f); AVSValueCollector::OnAVSValueCreate(this); }
	AVSValue::AVSValue(double f) { _value = new NativeAVSValue(f);  AVSValueCollector::OnAVSValueCreate(this); }
	AVSValue::AVSValue(String^ s) {
		_nts = gcnew NativeString(s);
		_value = new NativeAVSValue(_nts->GetPointer());
		AVSValueCollector::OnAVSValueCreate(this);
	}
	AVSValue::AVSValue(... array<AVSValue^>^ a) {
		_arrptr = (NativeAVSValue*)new NativeAVSValue[a->Length];
		for (int i = 0; i < a->Length; i++)
		{
			_arrptr[i] = NativeAVSValue(*(a[i]->_value));
		}
		_value = new NativeAVSValue(_arrptr, a->Length);
		AVSValueCollector::OnAVSValueCreate(this);
	}

	AVSValue::~AVSValue() {
		CleanUp();
		AVSValueCollector::OnAVSValueDispose(this);
	}
	AVSValue::!AVSValue() {
#ifdef DEBUG
		System::Diagnostics::Debug::WriteLine("AVSValue::!AVSValue begin");
#endif
		CleanUp();
#ifdef DEBUG
		System::Diagnostics::Debug::WriteLine("AVSValue::!AVSValue end");
#endif
	}
	void AVSValue::CollectorDispose() {
		CleanUp();
		GC::SuppressFinalize(this);
	}

	void AVSValue::CleanUp()
	{
		//try
		//{
		if (_value) {
			delete _value;
			_value = NULL;
		}
		if (_nts != nullptr) {
			delete _nts;
			_nts = nullptr;
		}
		if (_arrptr) {
			delete[] _arrptr;
			_arrptr = NULL;
		}
	}

	bool AVSValue::Defined() { return _value->Defined(); };
	bool AVSValue::IsClip() { return _value->IsClip(); };
	bool AVSValue::IsBool() { return _value->IsBool(); };
	bool AVSValue::IsInt() { return _value->IsInt(); };
	bool AVSValue::IsFloat() { return _value->IsFloat(); };
	bool AVSValue::IsString() { return _value->IsString(); };
	bool AVSValue::IsArray() { return _value->IsArray(); };

	Clip ^ AVSValue::AsClip() { return gcnew Clip(_value->AsClip()); }
	bool AVSValue::AsBool() { return _value->AsBool(); }
	int AVSValue::AsInt() { return _value->AsInt(); }
	String^ AVSValue::AsString() { return gcnew String(_value->AsString()); }
	double AVSValue::AsFloat() { return _value->AsFloat(); }

	bool AVSValue::AsBool(bool def) { return _value->AsBool(def); }
	int AVSValue::AsInt(int def) { return _value->AsInt(def); }
	double AVSValue::AsFloat(double def) { return _value->AsFloat((float)def); }
	String^ AVSValue::AsString(String^ def) { return IsString() ? gcnew String(_value->AsString()) : def; }

	int AVSValue::ArraySize() { return _value->ArraySize(); }

	AVSValue^ AVSValue::default::get(int index) {
		AVSValue^ ret = gcnew AVSValue((*_value)[index]);
		return ret;
	}
	AVSValue::AVSValue(NativeAVSValue value) {
		_value = new NativeAVSValue(value);
		AVSValueCollector::OnAVSValueCreate(this);
	};
	NativeAVSValue AVSValue::GetNative() {
		if (_nts) return NativeAVSValue(::_strdup(_nts->GetPointer()));
		return NativeAVSValue(*_value);
	};
}