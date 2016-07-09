#include "StdAfx.h"
#include "AVSValueCollector.h"

using namespace System::Threading;


namespace SAPStudio {
	namespace AvsFilterNet {
		AVSValueCollector::AVSValueCollector()
		{
			AVSValueCollector::BeginCollection();
		}

		AVSValueCollector::~AVSValueCollector()
		{
			AVSValueCollector::EndCollection();
		}

		void AVSValueCollector::OnAVSValueCreate(AVSValue^ value) {
			int id = Thread::CurrentThread->ManagedThreadId;
			if (!_values->ContainsKey(id)) return;
			if (_values[id]->Count == 0) return;
			_values[id]->Peek()->Add(value);
		}

		void AVSValueCollector::OnAVSValueDispose(AVSValue^ value) {
			int id = Thread::CurrentThread->ManagedThreadId;
			if (!_values->ContainsKey(id)) return;
			if (_values[id]->Count == 0) return;
			_values[id]->Peek()->Remove(value);
		}


		void AVSValueCollector::BeginCollection() {
			int id = Thread::CurrentThread->ManagedThreadId;
			if (!_values->ContainsKey(id)) {
				Monitor::Enter(AVSValueCollector::typeid);
				try
				{
					if (!_values->ContainsKey(id)) _values->Add(id, gcnew Stack<List<AVSValue^>^>);
				}
				finally
				{
					Monitor::Exit(AVSValueCollector::typeid);
				}
			}
			_values[id]->Push(gcnew List<AVSValue^>);
		}

		void AVSValueCollector::EndCollection() {
			List<AVSValue^>^ values = _values[Thread::CurrentThread->ManagedThreadId]->Pop();
			for (int i = 0; i < values->Count; i++)
			{
				values[i]->CollectorDispose();
			}
			values->Clear();
		}
	}
}