
#ifndef VIDEOFRAME_H
#define VIDEOFRAME_H


typedef VideoFrame NativeVideoFrame;
typedef PVideoFrame PNativeVideoFrame;

using namespace System::Runtime::InteropServices;
using namespace System;
using namespace System::IO;


namespace SAPStudio {
	namespace AvsFilterNet {

		public ref class VideoFrame sealed {
		private:
			NativeVideoFrame* _frame;
			PNativeVideoFrame* _pframe;
			void CleanUp();
		internal:
			VideoFrame(PNativeVideoFrame frame);
			~VideoFrame();

			PNativeVideoFrame GetNative();
			bool MakeWritable(IScriptEnvironment *env);
		protected:
			!VideoFrame();
		public:
			int GetPitch();
			int GetPitch(YUVPlanes plane);

			int GetRowSize();
			int GetRowSize(YUVPlanes plane);

			int GetHeight();
			int GetHeight(YUVPlanes plane);


			//// generally you shouldn't use these three
			//VideoFrameBuffer* GetFrameBuffer() const { return vfb; }
			//int GetOffset() const { return offset; }
			//int GetOffset(int plane) const { switch (plane) {case PLANAR_U: return offsetU;case PLANAR_V: return offsetV;default: return offset;}; }

			//// in plugins use env->SubFrame()
			//VideoFrame* Subframe(int rel_offset, int new_pitch, int new_row_size, int new_height) const;
			//VideoFrame* Subframe(int rel_offset, int new_pitch, int new_row_size, int new_height, int rel_offsetU, int rel_offsetV, int pitchUV) const;


			IntPtr GetReadPtr();
			IntPtr GetReadPtr(YUVPlanes plane);
            Stream^ GetReadStream();
            Stream^ GetReadStream(YUVPlanes plane);

			IntPtr GetWritePtr();
			IntPtr GetWritePtr(YUVPlanes plane);
            Stream^ GetWriteStream();
            Stream^ GetWriteStream(YUVPlanes plane);

			bool IsWritable();

		};
	}
}

#endif