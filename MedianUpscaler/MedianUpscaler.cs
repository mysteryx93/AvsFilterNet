﻿using System;
using System.IO;
using AvsFilterNet;

// http://forum.doom9.org/showthread.php?t=173787

[assembly: AvisynthFilterClass(typeof(Business.MedianUpscaler), "MedianUpscaler", "c", MtMode.NICE_FILTER)]
namespace Business {
    public class MedianUpscaler : AvisynthFilter {
        VideoInfo vi;

        public override void Initialize(AVSValue args, ScriptEnvironment env) {
            vi = GetVideoInfo();
            vi.height *= 2;
            vi.width *= 2;
            SetVideoInfo(ref vi);
        }

        public override VideoFrame GetFrame(int n, ScriptEnvironment env) {
            VideoFrame Src = Child.GetFrame(n, env);
            Src.GetReadPtr(YUVPlanes.PLANAR_Y);
            VideoFrame Dst = env.NewVideoFrame(ref vi);

            if (vi.IsPlanar()) {
                ProcessPlane(Src, Dst, YUVPlanes.PLANAR_Y);
                ProcessPlane(Src, Dst, YUVPlanes.PLANAR_U);
                ProcessPlane(Src, Dst, YUVPlanes.PLANAR_V);
            } else
                ProcessPlane(Src, Dst, YUVPlanes.PLANAR_Y);

            return Dst;
        }

        unsafe private void ProcessPlane(VideoFrame src, VideoFrame dst, YUVPlanes plane) {
            byte* SrcPtr = (byte*)src.GetReadPtr(plane);
            byte* DstPtr = (byte*)dst.GetWritePtr(plane);
            int SrcPitch = src.GetPitch(plane);
            int DstPitch = dst.GetPitch(plane);
            Step1(SrcPtr, DstPtr, SrcPitch, DstPitch);
            Step2(DstPtr, DstPitch);
            Step3(DstPtr, DstPitch);
        }

        /// <summary>
        /// Copy all source pixels from a w x h frame to 2w x 2h frame, with empty spaces between them.
        /// </summary>
        //              S   S   S   S
        //S S S S
        //S S S S  =>   S   S   S   S
        //S S S S
        //              S   S   S   S
        unsafe private void Step1(byte* src, byte* dst, int srcPitch, int dstPitch) {
            int Height = vi.height / 2;
            int Width = vi.width / 2;
            for (int y = 0; y < Height; y++) {
                for (int x = 0; x < Width; x++) {
                    dst[x * 2] = src[x];
                }

                src += srcPitch;
                dst += dstPitch * 2;
            }
        }

        /// <summary>
        /// Fill in all diagonal pixels by taking the median of their 4 diagonal neighbours.
        /// </summary>
        //S     S     S     S
        // \   / \   / \   / \
        //   M     M     M     M
        // /   \ /   \ /   \ /
        //S     S     S     S
        // \   / \   / \   / \
        //   M     M     M     M
        // /   \ /   \ /   \ /
        //S     S     S     S
        // \   / \   / \   / \
        //   M     M     M     M
        unsafe private void Step2(byte* dst, int dstPitch) {
            byte[] Val = new byte[4];
            int CalcHeight = vi.height - 2;
            int CalcWidth = vi.width - 2;
            for (int y = 0; y < CalcHeight; y += 2) {
                for (int x = 0; x < CalcWidth; x += 2) {
                    Val[0] = dst[x];
                    Val[1] = dst[x + 2];
                    Val[2] = dst[dstPitch * 2 + x];
                    Val[3] = dst[dstPitch * 2 + x + 2];
                    dst[dstPitch + x + 1] = (byte)(((int)Val[0] + Val[1] + Val[2] + Val[3]) / 4);
                }
                // Last column is average of 2
                Val[0] = dst[vi.width - 2];
                Val[1] = dst[dstPitch * 2 + vi.width - 2];
                dst[dstPitch + vi.width - 1] = (byte)(((int)Val[0] + Val[1]) / 2);

                dst += dstPitch * 2;
            }
            // Last row is average of 2
            for (int x = 0; x < CalcWidth; x += 2) {
                Val[0] = dst[x];
                Val[1] = dst[x + 2];
                dst[dstPitch + x + 1] = (byte)(((int)Val[0] + Val[1]) / 2);
            }
            // Lower-right corner has 1 neighbor value
            dst[dstPitch + vi.width - 1] = dst[vi.width - 2];
        }

        /// <summary>
        /// Now fill in all remaining blank pixels by, again, taking the median of the four existing neighbours.
        /// Half of all pixels need to be calculated here.
        /// </summary>
        //S -> o <- S -> o <- S
        //v    ^    v    ^    v
        //o <- M -> o <- M -> o
        //^    v    ^    v    ^
        //S -> o <- S -> o <- S
        //v    ^    v    ^    v
        //o <- M -> o <- M -> o
        //^    v    ^    v    ^
        //S -> o <- S -> o <- S
        //v    ^    v    ^    v
        unsafe private void Step3(byte* dst, int dstPitch) {
            byte[] Val = new byte[4];
            int CalcHeight = vi.height - 1;
            int CalcWidth = vi.width - 1;
            bool IsOddLine;
            // Todo: y=0
            dst += dstPitch;
            for (int y = 1; y < CalcHeight; y++) {
                IsOddLine = (y % 2) == 1;
                if (IsOddLine) {
                    // Todo: x=0
                }
                for (int x = IsOddLine ? 2 : 1; x < CalcWidth; x += 2) {
                    Val[0] = dst[x - 1];
                    Val[1] = dst[x + 1];
                    Val[2] = dst[x - dstPitch];
                    Val[3] = dst[x + dstPitch];
                    dst[x] = (byte)(((int)Val[0] + Val[1] + Val[2] + Val[3]) / 4);
                }
                if (!IsOddLine) {
                    // Todo: x=last
                }

                dst += dstPitch;
            }
            // Todo: y=last
            // Todo: x=0
            for (int x = 1; x < CalcWidth; x += 2) {
            }
        }
    }
}
