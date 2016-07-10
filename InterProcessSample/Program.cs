using System;
using System.ServiceModel;

namespace InterProcessSampleServer {
    public class Program {
        public const string Address = "net.pipe://localhost/InterProcessSample/SampleContract";

        static void Main(string[] args) {
            ServiceHost serviceHost = new ServiceHost(typeof(InterProcessSampleServer));
            NetNamedPipeBinding binding = new NetNamedPipeBinding(NetNamedPipeSecurityMode.None);
            binding.MaxReceivedMessageSize = 20000000;
            binding.MaxBufferPoolSize = 20000000;
            binding.MaxBufferSize = 20000000;
            binding.ReaderQuotas.MaxDepth = 32;
            binding.ReaderQuotas.MaxArrayLength = 20000000;
            binding.ReaderQuotas.MaxStringContentLength = 20000000;
            serviceHost.AddServiceEndpoint(typeof(IInterProcessSampleContract), binding, Address);
            try {
                serviceHost.Open();
                Console.WriteLine("ServiceHost running. Press Return to Exit");
                Console.ReadLine();
            }
            catch {
                // Already running
            }
        }
    }
}
