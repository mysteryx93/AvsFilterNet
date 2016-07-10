using System.IO;
using System.ServiceModel;
using System.Runtime.Serialization;

namespace InterProcessSampleServer {
    [ServiceContract(Namespace = "https://github.com/mysteryx93/AvsFilterNet")]
    public interface IInterProcessSampleContract {
        [OperationContract]
        byte[][] Add(RequestParameters e);
    }

    [DataContract()]
    public class RequestParameters {
        [DataMember()]
        public int FrameNumber { get; set; }
        [DataMember()]
        public byte[][] Source { get; set; }

        public RequestParameters(int frameNumber, Stream[] source) {
            this.FrameNumber = frameNumber;
            this.Source = new byte[source.GetLength(0)][];
            MemoryStream CopyStream;
            for (int i = 0; i < source.Length; i++) {
                CopyStream = new MemoryStream((int)source.Length);
                source[i].CopyTo(CopyStream);
                this.Source[i] = CopyStream.GetBuffer();
            }
        }
    }
}