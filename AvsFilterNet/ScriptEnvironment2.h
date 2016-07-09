#ifndef SCRIPTENVIRONMENT2_H
#define SCRIPTENVIRONMENT2_H

#include <stdlib.h>
using namespace System::Runtime::InteropServices;
using namespace System;
using namespace System::Collections::Generic;

namespace AvsFilterNet {
	ref class AVSValueCollector;
	//Wrapper of IScriptEnvironment
	public ref class ScriptEnvironment2 sealed {

	private:
		IScriptEnvironment2 * _env;

	internal:
		ScriptEnvironment2(IScriptEnvironment2 *env);
		IScriptEnvironment2 * GetNative();

	public:

		// Generic system to ask for various properties
		size_t GetProperty(AvsEnvProperty prop);

		// Returns TRUE and the requested variable. If the method fails, returns FALSE and does not touch 'val'.
		bool GetVar(String^ name, AVSValue^ val);

		// Return the value of the requested variable.
		// If the variable was not found or had the wrong type,
		// return the supplied default value.
		bool GetVar(String^ name, bool def);
		int  GetVar(String^ name, int def);
		double  GetVar(String^ name, double def);
		String^ GetVar(String^ name, String^ def);

		// Plugin functions
		bool LoadPlugin(String^ filePath, bool throwOnError, AVSValue^ result);
		void AddAutoloadDir(String^ dirPath, bool toFront);
		void ClearAutoloadDirs();
		void AutoloadPlugins();
		//void AddFunction(String^ name, String^ params, ApplyFunc apply, void* user_data, String^ exportVar);
		bool InternalFunctionExists(String^ name);

		// Threading
		void SetFilterMTMode(String^ filter, MtMode mode, bool force); // If filter is "", sets the default MT mode
		//IJobCompletion* NewCompletion(size_t capacity);
		//void ParallelJob(ThreadWorkerFuncPtr jobFunc, void* jobData, IJobCompletion* completion);

		// This version of Invoke will return false instead of throwing NotFound().
		bool Invoke(AVSValue^ result, String^ name, AVSValue^ args, array<String^>^ arg_names);

		// Support functions
		IntPtr Allocate(size_t nBytes, size_t alignment, AvsAllocType type);
		void Free(IntPtr ptr);
	};
}

#endif
