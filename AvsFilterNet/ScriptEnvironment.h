#ifndef SCRIPTENVIRONMENT_H
#define SCRIPTENVIRONMENT_H

#include <stdlib.h>
using namespace System::Runtime::InteropServices;
using namespace System;
using namespace System::Collections::Generic;

namespace AvsFilterNet {
	ref class AVSValueCollector;
	//Wrapper of IScriptEnvironment
	public ref class ScriptEnvironment sealed {

	private:
		IScriptEnvironment * _env;

	internal:
		ScriptEnvironment(IScriptEnvironment *env);
		IScriptEnvironment * GetNative();

	public:
		ScriptEnvironment2^ GetEnv2();
		CPUFlags GetCPUFlags();
		String^ SaveString(String ^s);
		String^ SaveString(String ^s, int length);
		void ThrowError(String ^str);
		//virtual void __stdcall AddFunction(const char* name, const char* params, ApplyFunc apply, void* user_data) = 0;
		//don't need to call AddFunction in managed code
		bool FunctionExists(String ^name);
		/// <summary>Invokes another function/filter. You must dispose all AVSValues used in this method, including return value. Use <see cref="AVSValueCollector"/> to automatically do this.</summary>
		AVSValue^ Invoke(String^ name, AVSValue ^args, array<String^> ^arg_names);
		/// <summary>Invokes another function/filter. You must dispose all AVSValues used in this method, including return value. Use <see cref="AVSValueCollector"/> to automatically do this.</summary>
		AVSValue^ Invoke(String ^name, AVSValue ^args);

		AVSValue^ GetVar(String^ name);
		bool SetVar(String^ name, AVSValue^ val);
		bool SetGlobalVar(String^ name, AVSValue^ val);

		void PushContext(int level);
		void PushContext();
		void PopContext();

		//// align should be 4 or 8
		VideoFrame^ NewVideoFrame(VideoInfo% vi);
		VideoFrame^ NewVideoFrame(VideoInfo% vi, int align);

		bool MakeWritable(VideoFrame^ pvf);

		void BitBlt(IntPtr dstp, int dst_pitch, IntPtr srcp, int src_pitch, int row_size, int height);
		void BitBlt(IntPtr dstp, int dst_pitch, array<BYTE>^ srcp, int src_pitch, int row_size, int height);
		void BitBlt(array<BYTE>^ dstp, int dst_pitch, IntPtr srcp, int src_pitch, int row_size, int height);
		void BitBlt(array<BYTE>^ dstp, int dst_pitch, array<BYTE>^ srcp, int src_pitch, int row_size, int height);

		//typedef void (__cdecl *ShutdownFunc)(void* user_data, IScriptEnvironment* env);
		//virtual void __stdcall AtExit(ShutdownFunc function, void* user_data) = 0;

		void CheckVersion(int version);

		VideoFrame^ Subframe(VideoFrame^ src, int rel_offset, int new_pitch, int new_row_size, int new_height);

		int SetMemoryMax(int mem);

		int SetWorkingDir(String^ newdir);

		IntPtr ManageCache(int key, IntPtr data);

		bool PlanarChromaAlignment(PlanarChromaAlignmentMode key);

		VideoFrame^ SubframePlanar(VideoFrame^ src, int rel_offset, int new_pitch, int new_row_size, int new_height, int rel_offsetU, int rel_offsetV, int new_pitchUV);
	};
};
#endif