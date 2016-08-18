LoadPlugin("C:\GitHub\AvsFilterNet\MedianUpscaler\bin\Debug\AvsFilterNet.dll")
LoadNetPlugin("C:\GitHub\AvsFilterNet\MedianUpscaler\bin\Debug\MedianUpscaler.dll")

ColorBarsHD(30, 20)
MedianUpscaler()
PointResize(Width*8, Height*8)