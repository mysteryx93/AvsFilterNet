LoadPlugin("C:\GitHub\AvsFilterNet\MedianUpscaler\bin\Debug\AvsFilterNet.dll")
LoadNetPlugin("C:\GitHub\AvsFilterNet\MedianUpscaler\bin\Debug\MedianUpscaler.dll")

LoadPlugin("F:\AVSMeter\TestImage\ImageSeq.dll")

#ColorBarsHD(30, 20)
ImageSource("F:\AVSMeter\TestImage\Lighthouse.png")
ConvertToYV24()
MedianUpscaler()
MedianUpscaler()
