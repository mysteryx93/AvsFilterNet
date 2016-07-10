LoadPlugin("C:\GitHub\AvsFilterNet\SimpleSampleNet\bin\x86\Debug\AvsFilterNet.dll")
#LoadNetPlugin("C:\GitHub\AvsFilterNet\SimpleSampleNet\bin\x86\Debug\SimpleSampleNet.dll")
LoadNetPlugin("C:\GitHub\AvsFilterNet\SimpleSampleNet\bin\x86\Debug\SimpleSampleNet.dll")

ColorBarsHD(600, 500)
#SetFilterMTMode("LoadImageNet", 2, true)
#LoadImageNet("F:\AVSMeter\TestImage\Lighthouse.png")
InterProcessNet("abc")
#Prefetch(4)