#include "stdafx.h"

using namespace System;

namespace SAPStudio {
    namespace AvsFilterNet {

        ScriptEnvironment2::ScriptEnvironment2(IScriptEnvironment2 *env) {
            _env = env;
        };
        IScriptEnvironment2 * ScriptEnvironment2::GetNative() {
            return _env;
        }

        size_t ScriptEnvironment2::GetProperty(AvsEnvProperty prop) {
            return _env->GetProperty(prop);
        }

        bool ScriptEnvironment2::GetVar(String^ name, AVSValue^ val) {
            NativeString^ nname = gcnew NativeString(name);
            try {
                val = gcnew AVSValue(_env->GetVar(nname->GetPointer()));
            }
            catch (IScriptEnvironment2::NotFound) {
                throw gcnew VarNotFoundException(name);
                return false;
            }
            finally {
                delete nname;
            }
            return true;
        }

        bool ScriptEnvironment2::GetVar(String^ name, bool def) {
            NativeString^ nname = gcnew NativeString(name);
            bool Result = _env->GetVar(nname->GetPointer(), def);
            delete nname;
            return Result;
        }

        int ScriptEnvironment2::GetVar(String^ name, int def) {
            NativeString^ nname = gcnew NativeString(name);
            int Result = _env->GetVar(nname->GetPointer(), def);
            delete nname;
            return Result;
        }

        double ScriptEnvironment2::GetVar(String^ name, double def) {
            NativeString^ nname = gcnew NativeString(name);
            double Result = _env->GetVar(nname->GetPointer(), def);
            delete nname;
            return Result;
        }

        String^ ScriptEnvironment2::GetVar(String^ name, String^ def) {
            NativeString^ nname = gcnew NativeString(name);
            NativeString^ ndef = gcnew NativeString(def);
            String^ Result = gcnew String(_env->GetVar(nname->GetPointer(), ndef->GetPointer()));
            delete nname;
            delete ndef;
            return Result;
        }

        bool ScriptEnvironment2::LoadPlugin(String^ filePath, bool throwOnError, AVSValue^ result) {
            NativeString^ nfilePath = gcnew NativeString(filePath);
            bool Result = false;
            try {
                Result = _env->LoadPlugin(nfilePath->GetPointer(), throwOnError, &result->GetNative());
            }
            catch (IScriptEnvironment2::NotFound) {
                throw gcnew FilterNotFoundException(filePath);
            }
            finally {
                delete nfilePath;
            }
            return Result;
        }

        void ScriptEnvironment2::ClearAutoloadDirs() {
            _env->ClearAutoloadDirs();
        }

        void ScriptEnvironment2::AddAutoloadDir(String ^ dirPath, bool toFront) {
            NativeString^ ndirPath = gcnew NativeString(dirPath);
            _env->AddAutoloadDir(ndirPath->GetPointer(), toFront);
            delete ndirPath;
        }

        void ScriptEnvironment2::AutoloadPlugins() {
            _env->AutoloadPlugins();
        }

        bool ScriptEnvironment2::InternalFunctionExists(String ^ name) {
            NativeString^ nname = gcnew NativeString(name);
            bool Result = _env->InternalFunctionExists(nname->GetPointer());
            delete nname;
            return Result;
        }

        void ScriptEnvironment2::SetFilterMTMode(String^ filter, MtMode mode, bool force) {
            NativeString^ nfilter = gcnew NativeString(filter);
            _env->SetFilterMTMode(nfilter->GetPointer(), (NativeMtMode)mode, force);
            delete nfilter;
        }

        bool ScriptEnvironment2::Invoke(AVSValue^ result, String^ name, AVSValue^ args, array<String^>^ arg_names) {
            NativeString^ nname = gcnew NativeString(name);
            List<NativeString^>^ n_arg_names;
            const char** p_arg_names = NULL;
            if (arg_names&&arg_names->Length>0) {
                n_arg_names = gcnew List<NativeString^>(arg_names->Length);
                p_arg_names = (const char**)malloc(sizeof(char*)*arg_names->Length);
                for (int i = 0; i < arg_names->Length; i++)
                {
                    if (String::IsNullOrEmpty(arg_names[i])) {
                        p_arg_names[i] = NULL;
                        continue;
                    }
                    NativeString^ str = gcnew NativeString(arg_names[i]);
                    p_arg_names[i] = str->GetPointer();
                    n_arg_names->Add(str);
                }
            }
            try
            {
                return _env->Invoke(&result->GetNative(), nname->GetPointer(), args->GetNative(), p_arg_names);
            }
            catch (AvisynthError err)
            {
                throw gcnew AvisynthException(Marshal::PtrToStringAnsi(IntPtr((void*)err.msg)));
            }
            finally
            {
                delete nname;
                if (p_arg_names) {
                    free(p_arg_names);
                    p_arg_names = NULL;
                    for (int i = 0; i < n_arg_names->Count; i++)
                    {
                        delete n_arg_names[i];
                    }
                }
            }
        }

        IntPtr ScriptEnvironment2::Allocate(size_t nBytes, size_t alignment, AvsAllocType type) {
            try
            {
                return (IntPtr)_env->Allocate(nBytes, alignment, type);
            }
            catch (AvisynthError err)
            {
                throw gcnew AvisynthException(Marshal::PtrToStringAnsi(IntPtr((void*)err.msg)));
            }
        }

        void ScriptEnvironment2::Free(IntPtr ptr) {
            try
            {
                _env->Free((void*)ptr);
            }
            catch (AvisynthError err)
            {
                throw gcnew AvisynthException(Marshal::PtrToStringAnsi(IntPtr((void*)err.msg)));
            }
        }
    };
};
