#include "stdafx.h"

using namespace System;

namespace AvsFilterNet {
	ScriptEnvironment::ScriptEnvironment(IScriptEnvironment *env) {
		_env = env;
	};

	IScriptEnvironment * ScriptEnvironment::GetNative() {
		return _env;
	}

	ScriptEnvironment2^ ScriptEnvironment::GetEnv2() {
		if (_env->FunctionExists("SetFilterMTMode"))
			return gcnew ScriptEnvironment2(static_cast<IScriptEnvironment2*>(_env));
		else
			return nullptr;
	}

	CPUFlags ScriptEnvironment::GetCPUFlags() {
		return (CPUFlags)Enum::ToObject(CPUFlags::typeid, (int)(_env->GetCPUFlags()));
	};

	String^ ScriptEnvironment::SaveString(String^ s) {
		return this->SaveString(s, -1);
	};

	String^ ScriptEnvironment::SaveString(String^ s, int length) {
		NativeString^ nts = gcnew NativeString(s);

		String^ ret = gcnew String(_env->SaveString(nts->GetPointer(), length));
		delete nts;

		return ret;
	};

	void ScriptEnvironment::ThrowError(String^ str) {
		try {
			System::IO::File::AppendAllText("AvsFilterNet.log", str);
		}
		catch (Exception^ ex) { (void)ex; }
		throw gcnew AvisynthException(str);
	}


	bool ScriptEnvironment::FunctionExists(String^ name) {
		NativeString^ nname = gcnew NativeString(name);

		bool ret = _env->FunctionExists(nname->GetPointer());
		delete nname;

		return ret;
	};

	AVSValue^ ScriptEnvironment::Invoke(String ^name, AVSValue ^args) {
		return Invoke(name, args, nullptr);
	}

	AVSValue^ ScriptEnvironment::Invoke(String^ name, AVSValue^ args, array<String^>^ arg_names) {
		NativeString^ nname = gcnew NativeString(name);
		List<NativeString^>^ n_arg_names;
		const char** p_arg_names = NULL;
		if (arg_names&&arg_names->Length > 0) {
			n_arg_names = gcnew List<NativeString^>(arg_names->Length);
			p_arg_names = (const char**)malloc(sizeof(char*)*arg_names->Length);
			for (int i = 0; i < arg_names->Length; i++) {
				if (String::IsNullOrEmpty(arg_names[i])) {
					p_arg_names[i] = NULL;
					continue;
				}
				NativeString^ str = gcnew NativeString(arg_names[i]);
				p_arg_names[i] = str->GetPointer();
				n_arg_names->Add(str);
			}
		}
		try {
			NativeAVSValue ret = NativeAVSValue(_env->Invoke(nname->GetPointer(), (args->GetNative()), p_arg_names));
			AVSValue^ retwrap = gcnew AVSValue(ret);
			return retwrap;
		}
		catch (IScriptEnvironment::NotFound) {
			throw gcnew FilterNotFoundException(name);
		}
		catch (AvisynthError err) {
			throw gcnew AvisynthException(Marshal::PtrToStringAnsi(IntPtr((void*)err.msg)));
		}
		finally {
			delete nname;
			if (p_arg_names) {
				free(p_arg_names);
				p_arg_names = NULL;
				for (int i = 0; i < n_arg_names->Count; i++) {
					delete n_arg_names[i];
				}
			}
		}
	}

	AVSValue ^ ScriptEnvironment::GetVar(String^ name) {
		NativeString^ nname = gcnew NativeString(name);
		try {
			AVSValue^ ret = gcnew AVSValue(_env->GetVar(nname->GetPointer()));
			return ret;
		}
		catch (IScriptEnvironment::NotFound) {
			throw gcnew VarNotFoundException(name);
		}
		finally {
			delete nname;
		}
	}

	bool ScriptEnvironment::SetVar(String^ name, AVSValue^ val) {
		NativeString^ nname = gcnew NativeString(name);
		bool ret = _env->SetVar(nname->GetPointer(), (val->GetNative()));
		delete nname;
		return ret;
	}

	bool ScriptEnvironment::SetGlobalVar(String^ name, AVSValue^ val) {
		NativeString^ nname = gcnew NativeString(name);
		bool ret = _env->SetGlobalVar(nname->GetPointer(), (val->GetNative()));
		delete nname;
		return ret;
	}

	void ScriptEnvironment::PushContext() {
		PushContext(0);
	}

	void ScriptEnvironment::PushContext(int level) {
		_env->PushContext(level);
	}

	void ScriptEnvironment::PopContext() {
		_env->PopContext();
	}

	VideoFrame^ ScriptEnvironment::NewVideoFrame(VideoInfo% vi) {
		return NewVideoFrame(vi, FRAME_ALIGN);
	}

	VideoFrame^ ScriptEnvironment::NewVideoFrame(VideoInfo% vi, int align) {
		pin_ptr<VideoInfo> pvi = &vi;
		return gcnew VideoFrame(_env->NewVideoFrame(*(NativeVideoInfo*)(void*)pvi, align));
	}

	bool ScriptEnvironment::MakeWritable(VideoFrame^ pvf) {
		return pvf->MakeWritable(_env);
	}

	void ScriptEnvironment::BitBlt(IntPtr dstp, int dst_pitch, IntPtr srcp, int src_pitch, int row_size, int height) {
		_env->BitBlt((BYTE*)dstp.ToPointer(), dst_pitch, (BYTE*)srcp.ToPointer(), src_pitch, row_size, height);
	}

	void ScriptEnvironment::BitBlt(IntPtr dstp, int dst_pitch, array<BYTE>^ srcp, int src_pitch, int row_size, int height) {
		pin_ptr<BYTE> srcm = &srcp[0];
		_env->BitBlt((BYTE*)dstp.ToPointer(), dst_pitch, (BYTE*)srcm, src_pitch, row_size, height);
	}

	void ScriptEnvironment::BitBlt(array<BYTE>^ dstp, int dst_pitch, IntPtr srcp, int src_pitch, int row_size, int height) {
		pin_ptr<BYTE> dstm = &dstp[0];
		_env->BitBlt((BYTE*)dstm, dst_pitch, (BYTE*)srcp.ToPointer(), src_pitch, row_size, height);
	}

	void ScriptEnvironment::BitBlt(array<BYTE>^ dstp, int dst_pitch, array<BYTE>^ srcp, int src_pitch, int row_size, int height) {
		pin_ptr<BYTE> dstm = &dstp[0];
		pin_ptr<BYTE> srcm = &srcp[0];
		_env->BitBlt((BYTE*)dstm, dst_pitch, (BYTE*)srcm, src_pitch, row_size, height);
	}

	void ScriptEnvironment::CheckVersion(int version) {
		_env->CheckVersion(version);
	}

	VideoFrame^ ScriptEnvironment::Subframe(VideoFrame^ src, int rel_offset, int new_pitch, int new_row_size, int new_height) {
		return gcnew VideoFrame(_env->Subframe(src->GetNative(), rel_offset, new_pitch, new_row_size, new_height));
	}

	int ScriptEnvironment::SetMemoryMax(int mem) {
		return _env->SetMemoryMax(mem);
	}

	int ScriptEnvironment::SetWorkingDir(String^ newdir) {
		NativeString^ nnewdir = gcnew NativeString(newdir);
		int ret = _env->SetWorkingDir(nnewdir->GetPointer());
		delete nnewdir;
		return ret;
	}

	IntPtr ScriptEnvironment::ManageCache(int key, IntPtr data) {
		return IntPtr(_env->ManageCache(key, data.ToPointer()));
	}

	bool ScriptEnvironment::PlanarChromaAlignment(PlanarChromaAlignmentMode key) {
		return _env->PlanarChromaAlignment((::IScriptEnvironment::PlanarChromaAlignmentMode)key);
	}

	VideoFrame^ ScriptEnvironment::SubframePlanar(VideoFrame^ src, int rel_offset, int new_pitch, int new_row_size, int new_height, int rel_offsetU, int rel_offsetV, int new_pitchUV) {
		return gcnew VideoFrame(_env->SubframePlanar(src->GetNative(), rel_offset, new_pitch, new_row_size, new_height, rel_offsetU, rel_offsetV, new_pitchUV));
	}
}