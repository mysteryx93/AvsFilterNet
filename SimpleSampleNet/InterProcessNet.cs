using System;
using System.Collections.Generic;
using System.IO;
using AvsFilterNet;
using System.ServiceModel;
using InterProcessSampleServer;
using System.Diagnostics;
using System.Reflection;

[assembly: AvisynthFilterClass(typeof(SimpleSampleNet.InterProcessNet), "InterProcessNet", "c[path]s", MtMode.SERIALIZED)]
namespace SimpleSampleNet {
    public class InterProcessNet : AvisynthFilter {
        VideoInfo vi;
        Process server;

        public override void Initialize(AVSValue args, ScriptEnvironment env) {
            vi = GetVideoInfo();
            string AppPath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            string ServerExePath = Path.Combine(AppPath, "InterProcessSample.exe");
            server = Process.Start(ServerExePath);
        }

        //public override AVSValue ExecuteBefore(AVSValue args, ref bool cancelLoad, ScriptEnvironment env) {
        //    return env.Invoke("FlipHorizontal", new AVSValue(args[0].AsClip()));
        //}

        //public override AVSValue ExecuteAfter(AVSValue clip, ScriptEnvironment env) {
        //    return env.Invoke("FlipVertical", new AVSValue(clip));
        //}

        protected override void Dispose(bool disposing) {
            if (!server.HasExited)
                server.Kill();
            base.Dispose(disposing);
        }

        public override VideoFrame GetFrame(int n, ScriptEnvironment env) {
            NetNamedPipeBinding binding = new NetNamedPipeBinding(NetNamedPipeSecurityMode.None);
            binding.MaxBufferSize = 20000000;
            EndpointAddress ep = new EndpointAddress(InterProcessSampleServer.Program.Address);
            IInterProcessSampleContract channel = ChannelFactory<IInterProcessSampleContract>.CreateChannel(binding, ep);

            VideoFrame Src = Child.GetFrame(n, env);
            Stream[] Planes = new Stream[] { Src.GetReadStream(YUVPlanes.PLANAR_Y), Src.GetReadStream(YUVPlanes.PLANAR_U), Src.GetReadStream(YUVPlanes.PLANAR_V) };
            byte[][] Result = channel.Add(new RequestParameters(n, Planes));
            VideoFrame Dst = NewVideoFrame(env);
            env.BitBlt(Dst.GetWritePtr(YUVPlanes.PLANAR_Y), Dst.GetPitch(YUVPlanes.PLANAR_Y), Result[0], Src.GetPitch(YUVPlanes.PLANAR_Y), Src.GetRowSize(YUVPlanes.PLANAR_Y), Src.GetHeight(YUVPlanes.PLANAR_Y));
            env.BitBlt(Dst.GetWritePtr(YUVPlanes.PLANAR_U), Dst.GetPitch(YUVPlanes.PLANAR_U), Result[1], Src.GetPitch(YUVPlanes.PLANAR_U), Src.GetRowSize(YUVPlanes.PLANAR_U), Src.GetHeight(YUVPlanes.PLANAR_U));
            env.BitBlt(Dst.GetWritePtr(YUVPlanes.PLANAR_V), Dst.GetPitch(YUVPlanes.PLANAR_V), Result[2], Src.GetPitch(YUVPlanes.PLANAR_V), Src.GetRowSize(YUVPlanes.PLANAR_V), Src.GetHeight(YUVPlanes.PLANAR_V));
            Src.Dispose(); // Why is this line necessary?
            return Dst;
        }
    }
}
