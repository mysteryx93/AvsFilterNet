#pragma once
using namespace System::Collections::Generic;

namespace AvsFilterNet {
	/// <summary>Simplifies AVSValue deallocation. When it is disposed, all AVSValue created in its life cycle will be disposed as well. Useful for env.Invoke calls.</summary>
	public ref class VideoFrameCollector {
	public:
		VideoFrameCollector();
		~VideoFrameCollector();

	internal:
		static void OnVideoFrameCreate(VideoFrame^ value);
		static void OnVideoFrameDispose(VideoFrame^ value);

	private:
		static void BeginCollection();
		static void EndCollection();

		static Dictionary<int, Stack<List<VideoFrame^>^>^>^ _values = gcnew Dictionary<int, Stack<List<VideoFrame^>^>^>;
	};
}
