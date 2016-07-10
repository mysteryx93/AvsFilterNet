// IPCTestServer.cs
using System;
using System.IO;
using System.ServiceModel;

namespace InterProcessSampleServer {
    [ServiceBehavior(InstanceContextMode = InstanceContextMode.Single)]
    public class InterProcessSampleServer : IInterProcessSampleContract {
        public byte[][] Add(RequestParameters e) {
            Console.WriteLine("Requested Frame #{0}, added {1}", e.FrameNumber, e.FrameNumber % 256);
            for (int p = 0; p < e.Source.GetLength(0); p++) {
                byte[] Src = e.Source[p];
                for (int i = 0; i < e.Source[p].Length; i++) {
                    Src[i] = (byte)((Src[i] + e.FrameNumber) % 256);
                }
            }
            return e.Source;
        }
    }
}