LoadPlugin("C:\GitHub\AvsFilterNet\SimpleSampleNet\bin\Debug\AvsFilterNet.dll")
#LoadNetPlugin("C:\GitHub\AvsFilterNet\SimpleSampleNet\bin\Debug\SimpleSampleNet.dll")
LoadNetPlugin("C:\GitHub\AvsFilterNet\SimpleSampleNet\bin\Debug\SimpleSampleNet.dll")

ColorBarsHD(800, 600)
SetFilterMTMode("LoadImageNet", 2, true)
LoadImageNet("F:\AVSMeter\TestImage\Lighthouse.png")
Prefetch(4)