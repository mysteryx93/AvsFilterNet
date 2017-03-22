using System;
using System.IO;
using System.Windows.Media.Imaging;
using AvsFilterNet;

[assembly: AvisynthFilterClass(typeof(SimpleSampleNet.LoadAllPlugins), "LoadAllPlugins", "c[path]s", MtMode.NICE_FILTER)]
namespace SimpleSampleNet {
    public class LoadAllPlugins : AvisynthFilter {
        VideoInfo vi;

        public override void Initialize(AVSValue args, ScriptEnvironment env) {
            vi = GetVideoInfo();
        }

        public override AVSValue ExecuteBefore(AVSValue args, ref bool cancelLoad, ScriptEnvironment env) {
            cancelLoad = true;
            string P = args[1].AsString("");
            if (P.Length > 0 && !P.EndsWith("\\"))
                P += "\\";
            env.Invoke("LoadPlugin", new AVSValue(P + "KNLMeansCL.dll"));
            return env.Invoke("KNLMeansCL", new AVSValue(args[0]));
        }
    }
}
