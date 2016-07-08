using System;
using SAPStudio.AvsFilterNet;
using System.Windows.Media.Imaging;

[assembly: AvisynthFilterClass(typeof(SAPStudio.SimpleSampleNet.LoadImageNet), "LoadImageNet", "[path]s")]
namespace SAPStudio.SimpleSampleNet {
    public class LoadImageNet : AvisynthFilter {
        VideoInfo vi;
        WriteableBitmap bitmap;

        public override AVSValue Initialize(AVSValue args, ScriptEnvironment env) {
            string path = args[0].AsString();
            BitmapImage BitmapFile = new BitmapImage(new Uri(path));
            bitmap = new WriteableBitmap(BitmapFile);
            vi.pixel_type = ColorSpaces.CS_BGR32;
            vi.fps_denominator = 1001;
            vi.fps_numerator = 30000;
            vi.height = (int)bitmap.Height;
            vi.width = (int)bitmap.Width;
            vi.num_frames = 1000;
            SetVideoInfo(ref vi);
            bitmap.Lock();
            return null;
        }

        protected override void Dispose(bool disposing) {
            if (bitmap != null)
                bitmap.Unlock();
            base.Dispose(disposing);
        }

        public override AVSValue Finalize(AVSValue clip, ScriptEnvironment env) {
            return env.Invoke("FlipVertical", new AVSValue(clip));
        }

        public override VideoFrame GetFrame(int n, ScriptEnvironment env) {
            VideoFrame dst = NewVideoFrame(env);
            env.BitBlt(dst.GetWritePtr(), dst.GetPitch(), bitmap.BackBuffer, bitmap.BackBufferStride, vi.width * 4, vi.height);
            return dst;
        }
    }
}
