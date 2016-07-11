#include "StdAfx.h"

using namespace System::Threading;

namespace AvsFilterNet {
	VideoFrameCollector::VideoFrameCollector()
	{
		VideoFrameCollector::BeginCollection();
	}

	VideoFrameCollector::~VideoFrameCollector()
	{
		VideoFrameCollector::EndCollection();
	}

	void VideoFrameCollector::OnVideoFrameCreate(VideoFrame^ value) {
		int id = Thread::CurrentThread->ManagedThreadId;
		if (!_values->ContainsKey(id)) return;
		if (_values[id]->Count == 0) return;
		_values[id]->Peek()->Add(value);
	}

	void VideoFrameCollector::OnVideoFrameDispose(VideoFrame^ value) {
		int id = Thread::CurrentThread->ManagedThreadId;
		if (!_values->ContainsKey(id)) return;
		if (_values[id]->Count == 0) return;
		_values[id]->Peek()->Remove(value);
	}


	void VideoFrameCollector::BeginCollection() {
		int id = Thread::CurrentThread->ManagedThreadId;
		if (!_values->ContainsKey(id)) {
			Monitor::Enter(VideoFrameCollector::typeid);
			try
			{
				if (!_values->ContainsKey(id)) _values->Add(id, gcnew Stack<List<VideoFrame^>^>);
			}
			finally
			{
				Monitor::Exit(VideoFrameCollector::typeid);
			}
		}
		_values[id]->Push(gcnew List<VideoFrame^>);
	}

	void VideoFrameCollector::EndCollection() {
		List<VideoFrame^>^ values = _values[Thread::CurrentThread->ManagedThreadId]->Pop();
		for (int i = 0; i < values->Count; i++)
		{
			values[i]->CollectorDispose();
		}
		values->Clear();
	}
}