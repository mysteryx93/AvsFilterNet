# AvsFilterNet

##### Project Description
The main objective of AvsFilterNet is to provide a wrapper for avisynth, making it possible to write simple filter in any .Net languages.

##### Writing a .NET filter
Create a .NET DLL project in Visual Studio.  
Add reference to AvsFilterNet.dll (Note: don't change its file name!)  
Implement your filter (see sample projects in the source code package for more details)  

##### Using a .NET filter
There are several ways to load a .NET filter into Avisynth environment:  
Manual loading:  
Use the following statements to manually load your .NET filter:  
LoadPlugin("path\to\AvsFilterNet.dll")  
LoadNetPlugin("path\to\filter.dll")  
Semi-auto loading:  
Rename your filter to have "_netautoload" in the file name (position doesn't matter), and put it into the same directory of   AvsFilterNet.dll, add the following line to your script:  
LoadPlugin("path\to\AvsFilterNet.dll")  
Auto loading  
Put AvsFilterNet.dll and renamed filter (see above) into your Avisynth plugins folder and you are done.  

##### Once the filter is loaded, you can use it like other regular filters in your script.


AvsFilterNet originally written by [SAPikachu](https://avsfilternet.codeplex.com/), upgraded to AviSynth+ by [Etienne Charland](https://www.spiritualselftransformation.com/).
