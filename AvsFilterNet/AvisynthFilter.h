
namespace AvsFilterNet {
	class AvisynthFilterNativeStub;

	public ref class AvisynthFilter abstract {
	private:
		AvisynthFilterNativeStub* _stub;
		Clip^ _child;
		bool _initialized;
		VideoInfo _vi;
	public:
		~AvisynthFilter();
		/// <summary>The constructor of <see cref="AvisynthFilter"/>.</summary>
		AvisynthFilter();

		/// <summary>The child clip. It will be used in methods that is not overrode or when null (or Nothing in Visual Basic) is returned in <see cref="GetFrame"/>.</summary>
		property Clip^ Child { Clip^ get(); };

		/// <summary>Sets a <see cref="Clip"/> as new child clip.</summary>
		/// <remarks>This method can only be called during filter initialization, otherwise an exception will be thrown.</remarks>
		void SetChild(Clip^ child);
		/// <summary>Sets new video info. If you want to use a output format different from that of child clip, you must call this method to specify it.</summary>
		/// <remarks>This method can only be called during filter initialization, otherwise an exception will be thrown. Additionally, don't call <see cref="SetChild"/> after new video info is set as it will be overwritten.</remarks>
		void SetVideoInfo(VideoInfo% vi);

		/// <summary>Called when the class is being initialized. Use this function instead of overriding the constructor.</summary>
		/// <param name='args'>The AviSynth arguments of the plugin.</param>
		/// <param name='env'>Script environment object.</param>
		virtual void Initialize(AVSValue^ args, ScriptEnvironment^ env);
		/// <summary>Called before the filter creation and allows executing pre-processing filters.</summary>
		/// <param name='clip'>The source clip of this filter.</param>
		/// <param name='cancelLoad'>If set to true, this plugin will not be executed. Useful if you're instead executing other plugins.</param>
		/// <param name='env'>Script environment object.</param>
		/// <returns>A <see cref="AVSValue"/>A <see cref="AVSValue"/>. The plugin chain to return to AviSynth for processing.</returns>
		virtual AVSValue^ ExecuteBefore(AVSValue^ args, bool% cancelLoad, ScriptEnvironment^ env);
		/// <summary>Called at the end of the filter creation and allows executing additional post-processing plugins.</summary>
		/// <param name='clip'>The output clip of this plugin.</param>
		/// <param name='env'>Script environment object.</param>
		/// <returns>A <see cref="AVSValue"/>A <see cref="AVSValue"/>. The plugin chain to return to AviSynth for processing.</returns>
		virtual AVSValue^ ExecuteAfter(AVSValue^ clip, ScriptEnvironment^ env);

		/// <summary>Called when the specified frame is requested.</summary>
		/// <param name='n'>Requested frame number.</param>
		/// <param name='env'>Script environment object.</param>
		/// <returns>A <see cref="VideoFrame"/>. If null (or Nothing in Visual Basic) is returned, the frame will be fetched from <see cref="Child"/>.</returns>
		virtual VideoFrame^ GetFrame(int n, ScriptEnvironment^ env);
		virtual void GetAudio(IntPtr buf, __int64 start, __int64 count, ScriptEnvironment^ env);
		virtual bool GetParity(int n);
		virtual int SetCacheHints(CacheType cachehints, int frame_range);  // We do not pass cache requests upwards, only to the next filter.

		VideoInfo GetVideoInfo();

		/// <summary>Creates a new <see cref="VideoFrame"/> using <see cref="VideoFrame"/> of the filter itself.</summary>
		VideoFrame^ NewVideoFrame(ScriptEnvironment^ env);
		/// <summary>Creates a new <see cref="VideoFrame"/> using <see cref="VideoFrame"/> of the filter itself, with specified frame align.</summary>
		VideoFrame^ NewVideoFrame(int align, ScriptEnvironment^ env);
	internal:
		PVideoFrame GetFrame(int n, IScriptEnvironment* env);
		void GetAudio(void* buf, __int64 start, __int64 count, IScriptEnvironment* env);
		void InitComplete();
		AvisynthFilterNativeStub* GetNativeStub();
	};
};
