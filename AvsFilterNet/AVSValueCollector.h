#pragma once
using namespace System::Collections::Generic;

namespace AvsFilterNet {
	/// <summary>Simplifies AVSValue deallocation. When it is disposed, all AVSValue created in its life cycle will be disposed as well. Useful for env.Invoke calls.</summary>
	public ref class AVSValueCollector {
	public:
		AVSValueCollector();
		~AVSValueCollector();

	internal:
		static void OnAVSValueCreate(AVSValue^ value);
		static void OnAVSValueDispose(AVSValue^ value);

	private:
		static void BeginCollection();
		static void EndCollection();

		static Dictionary<int, Stack<List<AVSValue^>^>^>^ _values = gcnew Dictionary<int, Stack<List<AVSValue^>^>^>;
	};
}
