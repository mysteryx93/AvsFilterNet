﻿using System;
using System.IO;
using System.Windows.Media.Imaging;
using SAPStudio.AvsFilterNet;

[assembly: AvisynthFilterClass(typeof(SAPStudio.SimpleSampleNet.LoadImageNet), "LoadImageNet", "c[path]s", MtMode.NICE_FILTER)]
namespace SAPStudio.SimpleSampleNet {
    public class LoadImageNet : AvisynthFilter {
        VideoInfo vi;
        MemoryStream imageStream;
        int imagePitch;

        public LoadImageNet():base() {
        }

        public override AVSValue Initialize(AVSValue args, ScriptEnvironment env) {
            string path = args[1].AsString();
            BitmapImage BitmapFile = new BitmapImage(new Uri(path));
            WriteableBitmap Bitmap = new WriteableBitmap(BitmapFile);

            vi.pixel_type = ColorSpaces.CS_BGR32;
            vi.fps_denominator = 1001;
            vi.fps_numerator = 30000;
            vi.height = (int)BitmapFile.Height;
            vi.width = (int)BitmapFile.Width;
            vi.num_frames = 1000;
            SetVideoInfo(ref vi);

            imagePitch = Bitmap.BackBufferStride;
            byte[] BitmapData = new byte[vi.height * imagePitch];
            Bitmap.CopyPixels(BitmapData, imagePitch, 0);
            imageStream = new MemoryStream(BitmapData, 0, vi.height * imagePitch, false, true);
            return null;
        }

        protected override void Dispose(bool disposing) {
            base.Dispose(disposing);
        }

        public override AVSValue Finalize(AVSValue clip, ScriptEnvironment env) {
            return env.Invoke("FlipVertical", new AVSValue(clip));
        }

        public override VideoFrame GetFrame(int n, ScriptEnvironment env) {
            lock (imageStream) {
                VideoFrame dst = NewVideoFrame(env);
                env.BitBlt(dst.GetWritePtr(), dst.GetPitch(), imageStream.GetBuffer(), imagePitch, vi.width * 4, vi.height);
                return dst;
            }
        }
    }
}
