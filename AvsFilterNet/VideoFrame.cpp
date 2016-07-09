#include "StdAfx.h"

namespace AvsFilterNet {
	//see Clip.cpp
	VideoFrame::VideoFrame(PNativeVideoFrame frame) {
		_pframe = new PNativeVideoFrame(frame);
		_frame = (NativeVideoFrame*)(void*)*_pframe;
	}

	VideoFrame::~VideoFrame() {
		CleanUp();
	}

	VideoFrame::!VideoFrame() {
#if DEBUG
		System::Diagnostics::Debug::WriteLine("VideoFrame::!VideoFrame start");
#endif
		CleanUp();
#if DEBUG
		System::Diagnostics::Debug::WriteLine("VideoFrame::!VideoFrame end");
#endif
	}

	void VideoFrame::CleanUp()
	{
		_frame = NULL;
		if (_pframe)
			delete _pframe;
		_pframe = NULL;
	}

	PNativeVideoFrame VideoFrame::GetNative() {
		return PNativeVideoFrame(_frame);
	}

	int VideoFrame::GetPitch() {
		return _frame->GetPitch();
	}

	int VideoFrame::GetPitch(YUVPlanes plane) {
		return _frame->GetPitch((int)plane);
	}

	int VideoFrame::GetRowSize() {
		return _frame->GetRowSize();
	}

	int VideoFrame::GetRowSize(YUVPlanes plane) {
		return _frame->GetRowSize((int)plane);
	}

	int VideoFrame::GetHeight() {
		return _frame->GetHeight();
	}

	int VideoFrame::GetHeight(YUVPlanes plane) {
		return _frame->GetHeight((int)plane);
	}

	IntPtr VideoFrame::GetReadPtr() {
		return IntPtr((void*)_frame->GetReadPtr());
	}

	IntPtr VideoFrame::GetReadPtr(YUVPlanes plane) {
		return IntPtr((void*)_frame->GetReadPtr((int)plane));
	}

	Stream^ VideoFrame::GetReadStream() {
		int frameSize = (_frame->GetPitch()*_frame->GetHeight()) & 0x7fffffff;
		return gcnew UnmanagedMemoryStream((unsigned char *)_frame->GetReadPtr(), frameSize, frameSize, FileAccess::Read);
	}

	Stream^ VideoFrame::GetReadStream(YUVPlanes plane) {
		int frameSize = (_frame->GetPitch((int)plane)*_frame->GetHeight((int)plane)) & 0x7fffffff;
		return gcnew UnmanagedMemoryStream((unsigned char *)_frame->GetReadPtr((int)plane), frameSize, frameSize, FileAccess::Read);
	}

	IntPtr VideoFrame::GetWritePtr() {
		return IntPtr((void*)_frame->GetWritePtr());
	}

	IntPtr VideoFrame::GetWritePtr(YUVPlanes plane) {
		return IntPtr((void*)_frame->GetWritePtr((int)plane));
	}

	Stream^ VideoFrame::GetWriteStream() {
		int frameSize = (_frame->GetPitch()*_frame->GetHeight()) & 0x7fffffff;
		return gcnew UnmanagedMemoryStream((unsigned char *)_frame->GetWritePtr(), frameSize, frameSize, FileAccess::Write);
	}

	Stream^ VideoFrame::GetWriteStream(YUVPlanes plane) {
		int frameSize = (_frame->GetPitch((int)plane)*_frame->GetHeight((int)plane)) & 0x7fffffff;
		return gcnew UnmanagedMemoryStream((unsigned char *)_frame->GetWritePtr((int)plane), frameSize, frameSize, FileAccess::Write);
	}

	bool VideoFrame::IsWritable() {
		return _frame->IsWritable();
	}

	bool VideoFrame::MakeWritable(IScriptEnvironment *env) {
		bool ret = env->MakeWritable(_pframe);
		_frame = (NativeVideoFrame*)(void*)*_pframe;
		return ret;
	}
}