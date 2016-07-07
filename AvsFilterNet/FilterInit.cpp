#include "Stdafx.h"


#include "FilterInit.h"

using namespace System::Diagnostics;
using namespace System::Reflection;
using namespace System;
//
//ref class TestPlugin : SAPStudio::AvsFilterNet::AvisynthFilter {
//public:
//	TestPlugin(SAPStudio::AvsFilterNet::AVSValue^ args, SAPStudio::AvsFilterNet::ScriptEnvironment^ env) : SAPStudio::AvsFilterNet::AvisynthFilter(args,env) {
//		Debug::WriteLine("TestPlugin: Init");
//		//SAPStudio::AvsFilterNet::AVSValue^ chargs=gcnew SAPStudio::AvsFilterNet::AVSValue(gcnew array<SAPStudio::AvsFilterNet::AVSValue^>{gcnew SAPStudio::AvsFilterNet::AVSValue(Child),gcnew SAPStudio::AvsFilterNet::AVSValue("TestPlugin"),gcnew SAPStudio::AvsFilterNet::AVSValue(30),gcnew SAPStudio::AvsFilterNet::AVSValue(30)});
//		//this->SetChild(env->Invoke("Subtitle",chargs,gcnew array<String^>{"","","y","size"})->AsClip());
//		//SAPStudio::AvsFilterNet::VideoInfo vi=this->GetVideoInfo();
//		//Debug::WriteLine("TestPlugin: Video width: " + vi.width);
//		//Debug::WriteLine("TestPlugin: Video height: " + vi.height);
//		//Debug::WriteLine("TestPlugin: Frame count: " + vi.num_frames);
//		//Debug::WriteLine("TestPlugin: Frames per second: " + vi.fps_numerator + "/" + vi.fps_denominator);
//		//Debug::WriteLine("TestPlugin: Pixel type: " + vi.pixel_type.ToString());
//		//Debug::WriteLine("TestPlugin: Audio samples per second: " + vi.audio_samples_per_second);
//		//Debug::WriteLine("TestPlugin: Audio sample type: " + vi.sample_type.ToString());
//		//Debug::WriteLine("TestPlugin: Audio sample count: " + vi.num_audio_samples);
//		//Debug::WriteLine("TestPlugin: Audio channels: " + vi.nchannels);
//		//Debug::WriteLine("TestPlugin: Image type: " + vi.image_type.ToString());
//		//Debug::WriteLine("TestPlugin: GetCPUFlags: " + env->GetCPUFlags().ToString());
//		////env->ThrowError("TestPlugin");
//		//vi.num_frames/=4;
//		//vi.fps_numerator/=4;
//		//this->SetVideoInfo(vi);
//	};
//
//	virtual SAPStudio::AvsFilterNet::VideoFrame^  GetFrame(int n, SAPStudio::AvsFilterNet::ScriptEnvironment^ env) override {
//		//Debug::WriteLine("TestPlugin: GetFrame " + n);
//		//SAPStudio::AvsFilterNet::VideoFrame^ src = Child->GetFrame(n*4,env);
//		//SAPStudio::AvsFilterNet::VideoFrame^ dst = this->NewVideoFrame();
//		//SAPStudio::AvsFilterNet::YUVPlanes Y = SAPStudio::AvsFilterNet::YUVPlanes::PLANAR_Y;
//		//env->BitBlt(dst->GetWritePtr(Y),dst->GetPitch(Y),src->GetReadPtr(Y),src->GetPitch(Y),src->GetRowSize(Y),src->GetHeight(Y)/2);
//		//System::IO::Stream^ ys=dst->GetWriteStream(Y);
//		//array<byte>^ line = gcnew array<byte>(dst->GetRowSize(Y));
//		//ys->Seek(dst->GetHeight(Y)/2*dst->GetPitch(Y),System::IO::SeekOrigin::Begin);
//		//int modulo=dst->GetPitch(Y)-dst->GetRowSize(Y);
//		//for (int i = 0; i < src->GetHeight(Y)/2; i++)
//		//{
//		//	ys->Write(line,0,line->Length);
//		//	ys->Seek(modulo,System::IO::SeekOrigin::Current);
//		//}
//		//SAPStudio::AvsFilterNet::YUVPlanes U = SAPStudio::AvsFilterNet::YUVPlanes::PLANAR_U;
//		//env->BitBlt(dst->GetWritePtr(U),dst->GetPitch(U),src->GetReadPtr(U),src->GetPitch(U),src->GetRowSize(U),src->GetHeight(U));
//		//SAPStudio::AvsFilterNet::YUVPlanes V = SAPStudio::AvsFilterNet::YUVPlanes::PLANAR_V;
//		//env->BitBlt(dst->GetWritePtr(V),dst->GetPitch(V),src->GetReadPtr(V),src->GetPitch(V),src->GetRowSize(V),src->GetHeight(V));
//		//delete src;
//		//return dst;
//		SAPStudio::AvsFilterNet::YUVPlanes Y = SAPStudio::AvsFilterNet::YUVPlanes::PLANAR_Y;
//		SAPStudio::AvsFilterNet::VideoFrame^ dst = Child->GetFrame(n,env);
//		env->MakeWritable(dst);
//		System::IO::Stream^ ys=dst->GetWriteStream(Y);
//		array<byte>^ line = gcnew array<byte>(dst->GetRowSize(Y));
//		ys->Seek(dst->GetHeight(Y)/2*dst->GetPitch(Y),System::IO::SeekOrigin::Begin);
//		int modulo=dst->GetPitch(Y)-dst->GetRowSize(Y);
//		for (int i = 0; i < dst->GetHeight(Y)/2; i++)
//		{
//			ys->Write(line,0,line->Length);
//			ys->Seek(modulo,System::IO::SeekOrigin::Current);
//		}
//		delete ys;
//		return dst;
//	};
//
//	//virtual void GetAudio(IntPtr buf, __int64 start, __int64 count, SAPStudio::AvsFilterNet::ScriptEnvironment^ env) override {
//	//	Debug::WriteLine(String::Format("TestPlugin: GetAudio {0}, {1}",start,count));
//	//	Child->GetAudio(buf,start,count,env);
//	//}
//
//	virtual ~TestPlugin(){
//		Debug::WriteLine("TestPlugin: Dispose");
//	}
//};
//
//NativeAVSValue __cdecl Create_TestPlugin(NativeAVSValue args, void* user_data, IScriptEnvironment* env) {
//	return (gcnew TestPlugin(gcnew SAPStudio::AvsFilterNet::AVSValue(args),gcnew SAPStudio::AvsFilterNet::ScriptEnvironment(env)))->GetNativeStub();  
//    // Calls the constructor with the arguments provied.
//}


NativeAVSValue CreateNetPluginImpl(NativeAVSValue &args, void *user_data, IScriptEnvironment *env)
{
	using namespace SAPStudio::AvsFilterNet;
	Type ^ filterType = (Type ^)GCHandle::FromIntPtr(IntPtr(user_data)).Target;
	AVSValueCollector^ col = gcnew AVSValueCollector;
	try
	{
		// Calls the constructor with the arguments provied.
        SAPStudio::AvsFilterNet::ScriptEnvironment^ envM = gcnew SAPStudio::AvsFilterNet::ScriptEnvironment(env);
        AvisynthFilter^ Filter = ((AvisynthFilter^)Activator::CreateInstance(filterType, gcnew SAPStudio::AvsFilterNet::AVSValue(args), envM));
        return Filter->GetNativeStub();
        //return gcnew SAPStudio::AvsFilterNet::AVSValue(Filter->GetNativeStub())->GetNative();
        //return Filter->Closing(envM)->GetNative();
	}
	catch (AvisynthError err) 
	{
		// To prevent problems, we always duplicate error messages
		env->ThrowError(_strdup(err.msg));
	}
	catch (AvisynthException^ ex){
		env->ThrowError((char *)Marshal::StringToHGlobalAnsi(ex->Message).ToPointer());
	}
	catch (Exception^ ex){
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


void LoadNetPluginImpl(String^ path, IScriptEnvironment* env, bool throwErr){
	using namespace SAPStudio::AvsFilterNet;
	try
	{
		Assembly^ assm = Assembly::LoadFrom(path);
		array<Object^> ^ attrs = assm->GetCustomAttributes(AvisynthFilterClassAttribute::typeid, true);
		array<Type^> ^ ctorParams = gcnew array<Type^>(2);
		ctorParams[0] = SAPStudio::AvsFilterNet::AVSValue::typeid;
		ctorParams[1] = SAPStudio::AvsFilterNet::ScriptEnvironment::typeid;
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
				env->AddFunction(name,params,Create_NetPlugin,handle);
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
	LoadNetPluginImpl(gcnew String(args[0].AsString()),env,true);
	return NativeAVSValue();  
    // Calls the constructor with the arguments provied.
}

Assembly^ ResolveAssembly( Object^ sender, ResolveEventArgs^ args )
   {
	   Assembly^ assm = Assembly::GetExecutingAssembly();
	   if (args->Name->StartsWith(assm->GetName()->Name + ",")) {
		   return assm;
	   }
	   return nullptr;
   }

void AutoLoadPlugins(IScriptEnvironment* env){
	array<String^> ^ fileNames = Directory::GetFiles(Path::GetDirectoryName(Assembly::GetExecutingAssembly()->Location),"*_netautoload*.dll");
	for (int i = 0; i < fileNames->Length; i++)
	{
		LoadNetPluginImpl(fileNames[i],env,false);
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
	env->AddFunction("LoadNetPlugin","s",LoadNetPlugin,0);
	AutoLoadPlugins(env);
    return "AvsFilterNet";
    // A freeform name of the plugin.
};
