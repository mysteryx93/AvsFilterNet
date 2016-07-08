#include "Stdafx.h"
#include "FilterInit.h"

using namespace System::Diagnostics;
using namespace System::Reflection;
using namespace System;

NativeAVSValue CreateNetPluginImpl(NativeAVSValue &args, void *user_data, IScriptEnvironment *env)
{
	using namespace SAPStudio::AvsFilterNet;
	Type ^ filterType = (Type ^)GCHandle::FromIntPtr(IntPtr(user_data)).Target;
	AVSValueCollector^ col = gcnew AVSValueCollector;
	try
	{
		// Calls the constructor with the arguments provied.
		SAPStudio::AvsFilterNet::ScriptEnvironment^ envM = gcnew SAPStudio::AvsFilterNet::ScriptEnvironment(env);
		AvisynthFilter^ Filter = ((AvisynthFilter^)Activator::CreateInstance(filterType));
		if (args[0].IsClip())
			Filter->SetChild(gcnew Clip(args[0].AsClip()));
		// If Initialize returns a value, we'll return that value and skip this class.
		SAPStudio::AvsFilterNet::AVSValue^ CancelLoad = Filter->Initialize(gcnew SAPStudio::AvsFilterNet::AVSValue(args), envM);
		if (CancelLoad) {
			Filter->SetChild(nullptr);
			return CancelLoad->GetNative();
		}
		else {
			// If we're using this class, Finalize will be called at the end.
			SAPStudio::AvsFilterNet::AVSValue^ V = gcnew SAPStudio::AvsFilterNet::AVSValue(gcnew Clip(Filter->GetNativeStub()));
			return Filter->Finalize(V, envM)->GetNative();
		}
	}
	catch (AvisynthError err)
	{
		// To prevent problems, we always duplicate error messages
		env->ThrowError(_strdup(err.msg));
	}
	catch (AvisynthException^ ex) {
		env->ThrowError((char *)Marshal::StringToHGlobalAnsi(ex->Message).ToPointer());
	}
	catch (Exception^ ex) {
		env->ThrowError((char *)Marshal::StringToHGlobalAnsi(filterType->Name + " initialization error: " + ex->ToString()).ToPointer());
	}
	finally
	{
		delete col;
	}
	return NativeAVSValue();
}

NativeAVSValue __cdecl Create_NetPlugin(NativeAVSValue args, void* user_data, IScriptEnvironment* env) {
	try
	{
		return CreateNetPluginImpl(args, user_data, env);
	}
	finally
	{
		System::GC::Collect(2);
	}
}


void LoadNetPluginImpl(String^ path, IScriptEnvironment* env, bool throwErr) {
	using namespace SAPStudio::AvsFilterNet;
	try
	{
		Assembly^ assm = Assembly::LoadFrom(path);
		array<Object^> ^ attrs = assm->GetCustomAttributes(AvisynthFilterClassAttribute::typeid, true);
		array<Type^> ^ ctorParams = gcnew array<Type^>(0);
		//ctorParams[0] = SAPStudio::AvsFilterNet::AVSValue::typeid;
		//ctorParams[1] = SAPStudio::AvsFilterNet::ScriptEnvironment::typeid;
		for (int i = 0; i < attrs->Length; i++)
		{
			try
			{
				AvisynthFilterClassAttribute^ attr = (AvisynthFilterClassAttribute^)attrs[i];
				//validate the plugin
				//must inherit from AvisynthFilter
				if (!attr->FilterType->IsSubclassOf(AvisynthFilter::typeid)) continue;
				//must have specified constructor
				ConstructorInfo^ ci = attr->FilterType->GetConstructor(ctorParams);
				//array<ConstructorInfo^>^ cis = attr->FilterType->GetConstructors();
				if (ci == nullptr) continue;

				//save type handle as user data
				void* handle;
				//Marshal::StructureToPtr(attr->FilterType->TypeHandle,IntPtr((void*)&typeHandle),false);
				handle = GCHandle::ToIntPtr(GCHandle::Alloc(attr->FilterType)).ToPointer();
				char *name, *params;
				name = (char *)Marshal::StringToHGlobalAnsi(attr->FilterName).ToPointer();
				params = (char *)Marshal::StringToHGlobalAnsi(attr->Arguments).ToPointer();
				env->AddFunction(name, params, Create_NetPlugin, handle);
				if (attr->MultiThreadingMode != SAPStudio::AvsFilterNet::MtMode::UNKNOWN && env->FunctionExists("SetFilterMTMode")) {
					auto env2 = static_cast<IScriptEnvironment2*>(env);
					env2->SetFilterMTMode(name, (NativeMtMode)attr->MultiThreadingMode, true);
				}
				Marshal::FreeHGlobal(IntPtr(name));
				Marshal::FreeHGlobal(IntPtr(params));
			}
			catch (Exception^)
			{
				//just ignore
			}
		}
	}
	catch (Exception ^)
	{
		if (throwErr) env->ThrowError("Unable to load plugin, make sure it is a valid .Net assembly");
	}
}

NativeAVSValue __cdecl LoadNetPlugin(NativeAVSValue args, void* user_data, IScriptEnvironment* env) {
	LoadNetPluginImpl(gcnew String(args[0].AsString()), env, true);
	return NativeAVSValue();
	// Calls the constructor with the arguments provied.
}

Assembly^ ResolveAssembly(Object^ sender, ResolveEventArgs^ args)
{
	Assembly^ assm = Assembly::GetExecutingAssembly();
	if (args->Name->StartsWith(assm->GetName()->Name + ",")) {
		return assm;
	}
	return nullptr;
}

void AutoLoadPlugins(IScriptEnvironment* env) {
	array<String^> ^ fileNames = Directory::GetFiles(Path::GetDirectoryName(Assembly::GetExecutingAssembly()->Location), "*_netautoload*.dll");
	for (int i = 0; i < fileNames->Length; i++)
	{
		LoadNetPluginImpl(fileNames[i], env, false);
	}
}

const AVS_Linkage *AVS_linkage = 0;

extern "C" __declspec(dllexport) const char* __stdcall AvisynthPluginInit3(IScriptEnvironment* env, const AVS_Linkage* const vectors) {
	AVS_linkage = vectors;
	//env->AddFunction("SimpleSample", "c", Create_SimpleSample, 0);
	// The AddFunction has the following paramters:
	// AddFunction(Filtername , Arguments, Function to call,0);

	// Arguments is a string that defines the types and optional names of the arguments for your filter.
	// c - Video Clip
	// i - Integer number
	// f - Float number
	// s - String
	// b - boolean
	//env->AddFunction("TestNet","c",Create_TestPlugin,0);
	AppDomain::CurrentDomain->AssemblyResolve += gcnew ResolveEventHandler(ResolveAssembly);
	env->AddFunction("LoadNetPlugin", "s", LoadNetPlugin, 0);
	AutoLoadPlugins(env);
	return "AvsFilterNet";
	// A freeform name of the plugin.
};
