/* 
    This is SimpleSample plugin ported to .Net
    See http://avisynth.org/mediawiki/Filter_SDK/Simple_sample_1.7 for details
  
    Originally written by Simon Walters (siwalters@hotmail.com)
    .Net version coded by SAPikachu (sapikachu@gmail.com)
    Most comments are copied from the original code
  
  	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using SAPStudio.AvsFilterNet;


// The following attribute is used by the loader to find your plugin class.
// You can specify more than one instance of this attribute to have multiple plugins in one assembly.
// Attribute signature: AvisynthFilterClass(Type type, String filterName, String arguments)
// Parameters:
//          type: Your plugin type. It must be derived from AvisynthFilter.
//    filterName: Plugin name. This is the name used in AVS scripts.
//     arguments: Plugin arguments. It is a string that defines the types and optional names of the
//                                  arguments for you filter.
//                                         c - Video Clip
//                                         i - Integer number
//                                         f - Float number
//                                         s - String
//                                         b - boolean

//                                 The word inside the [ ] lets you used named parameters in your script
//                                 e.g last=SimpleSample(last,windowclip,size=100).
//                                 but last=SimpleSample(last,windowclip, 100) will also work 

// Any type can be followed with a '*' or '+' to indicate "zero or more" or "one or more" respectively. 
// In this case all the matching arguments will be gathered into a sub-array.
// For example, if your type string is "is+f", then the integer argument will be args[0], 
// the string arguments will be args[1][0], args[1][1], etc. , and the float argument will be args[2].

// '.' matches a single argument of any type. To match multiple arguments of any type, use ".*" or ".+". 
[assembly: AvisynthFilterClass(typeof(SAPStudio.SimpleSampleNet.SimpleSampleNet), "SimpleSample", "cc[SIZE]i")]
namespace SAPStudio.SimpleSampleNet {
    public class SimpleSampleNet : AvisynthFilter {
        // SimpleSample defines the name of your filter class. 
        // This name is only used internally, and does not affect the name of your filter or similar.
        // All .Net filters must inherit AvisynthFilter.
        Clip WindowVideo;
        int SquareSize;
        VideoInfo vi;

        public SimpleSampleNet() : base() {
            // This is the constructor. It does not return any value, and is always used, 
            //  when an instance of the class is created.

            //The signature of constructor must be (AVSValue args, ScriptEnvironment env),
            //otherwise the loader won't recognize your plugin.

            //Save the parameter for future use.
            //Remember index 0 is the child clip saved in base class.
        }

        public override AVSValue Initialize(AVSValue args, ScriptEnvironment env) {
            WindowVideo = args[1].AsClip();
            SquareSize = args[2].AsInt(0);
            vi = GetVideoInfo();
            return null;
        }

        protected override void Dispose(bool disposing) {
            // This is where you can deallocate any memory you might have used.
            if (disposing) {
                //Release any managed resources here.
                //NOTE: You must dispose all Clips(except for the Child clip since it is disposed in base class) 
                //when they are no longer needed, otherwise avisynth may crash at exit.
                WindowVideo.Dispose();
            }

            //Release any unmanaged resources here.

            //Don't forget to call this
            base.Dispose(disposing);
        }

        public override VideoFrame GetFrame(int n, ScriptEnvironment env) {
            // This is the function that AviSynth calls to get a given frame.
            // So when this functions gets called, the filter is supposed to return frame n.

            VideoFrame src = Child.GetFrame(n, env);
            // Request frame 'n' from the child (source) clip.
            VideoFrame window = WindowVideo.GetFrame(n, env);
            // Request frame "'n" from the WindowVideo clip
            VideoFrame dst = NewVideoFrame(env);
            // Construct a frame based on the information of the current frame.

            Stream srcp = src.GetReadStream();
            // Request a Read pointer from the source frame.
            // This will return the position of the upperleft pixel in YUY2 images,
            // and return the lower-left pixel in RGB.
            // RGB images are stored upside-down in memory. 
            // You should still process images from line 0 to height.
            // Note: In this sample we use Stream class to manipulate the buffer, 
            //       although you can still use pointer in C#.

            Stream dstp = dst.GetWriteStream();
            // Request a Write pointer from the newly created destination image.
            // You can request a writepointer to images that have just been
            // created by NewVideoFrame. If you recieve a frame from Clip.GetFrame(...)
            // you must call env.MakeWritable(frame) be recieve a valid write pointer.

            int dst_pitch = dst.GetPitch();
            // Requests pitch (length of a line) of the destination image.
            // For more information on pitch see: http://www.avisynth.org/index.php?page=WorkingWithImages
            // (short version - pitch is always equal to or greater than width to allow for seriously fast assembly code)

            int dst_width = dst.GetRowSize();
            // Requests rowsize (number of used bytes in a line.
            // See the link above for more information.

            int dst_height = dst.GetHeight();
            // Requests the height of the destination image.

            int src_pitch = src.GetPitch();
            int src_width = src.GetRowSize();
            int src_height = src.GetHeight();

            Stream windowp = window.GetReadStream();
            int window_pitch = window.GetPitch();
            int window_width = window.GetRowSize();
            int window_height = window.GetHeight();
            // Get info on the Windowed Clip (see src definitions for more information)


            int w, h;

            // This version of SimpleSample is intended to show how to utilise information from 2 clips in YUY2
            // colourspace only.  The original V1.6 code has been left in place fro all other
            // colourspaces.
            // It is designed purely for clarity and not as good or clever code :-)


            if (vi.IsRGB24()) {
                // The code just deals with RGB24 colourspace where each pixel is represented by
                // 3 bytes, Blue, Green and Red.
                // Although this colourspace is the easiest to understand, it is very rarely used because
                // a 3 byte sequence (24bits) cannot be processed easily using normal 32 bit registers.

                for (h = 0; h < src_height; h++) {       // Loop from bottom line to top line.
                    for (w = 0; w < src_width; w += 3) {   // Loop from left side of the image to the right side 1 pixel (3 bytes) at a time
                                                           // stepping 3 bytes (a pixel width in RGB24 space)

                        dstp.WriteByte((byte)srcp.ReadByte());                  // Copy each Blue byte from source to destination.
                        dstp.WriteByte((byte)srcp.ReadByte());     // Copy Green.
                        dstp.WriteByte((byte)srcp.ReadByte());      // Copy Red

                        //Note: The code above is for demonstration only. You should copy 1 line 
                        //      at a time for better performance. The same applies to codes below.
                    }

                    srcp.Seek(src_pitch - src_width, SeekOrigin.Current); // Seek to next line.
                    dstp.Seek(dst_pitch - dst_width, SeekOrigin.Current);
                }
                // end copy src to dst

                //Now draw a white square in the middle of the frame
                // Normally you'd do this code within the loop above but here it is in a separate loop for clarity;

                dstp.Seek((dst_height / 2 - SquareSize / 2) * dst_pitch, SeekOrigin.Begin);  // move pointer to SquareSize/2 lines from the middle of the frame;
                for (h = 0; h < SquareSize; h++) { // only scan SquareSize lines 
                    dstp.Seek(dst_width / 2 - SquareSize / 2 * 3, SeekOrigin.Current); // seek to left edge of the square
                    for (w = 0; w < SquareSize; w += 1) { // only scans the middle SquareSize pixels of a line 
                        dstp.WriteByte(255);        // Set Blue to maximum value.
                        dstp.WriteByte(255);     // and Green.
                        dstp.WriteByte(255);        // and Red - therefore the whole pixel is now white.
                    }
                    dstp.Seek(dst_width / 2 - SquareSize / 2 * 3, SeekOrigin.Current);//seek to right edge of the frame				
                    dstp.Seek(dst_pitch - dst_width, SeekOrigin.Current); //seek to next line
                }
            }

            if (vi.IsRGB32()) {
                // This code deals with RGB32 colourspace where each pixel is represented by
                // 4 bytes, Blue, Green and Red and "spare" byte that could/should be used for alpha
                // keying but usually isn't.

                // Although this colourspace isn't memory efficient, code end ups running much
                // quicker than RGB24 as you can deal with whole 32bit variables at a time
                // and easily work directly and quickly in assembler (if you know how to that is :-)
                byte[] buffer = new byte[4];
                for (h = 0; h < src_height; h++) {       // Loop from bottom line to top line.
                    for (w = 0; w < src_width / 4; w += 1) {    // and from leftmost pixel to rightmost one. 
                        srcp.Read(buffer, 0, 4);
                        dstp.Write(buffer, 0, 4);// Copy each whole pixel from source to destination.
                    }
                    srcp.Seek(src_pitch - src_width, SeekOrigin.Current); // Seek to next line.
                    dstp.Seek(dst_pitch - dst_width, SeekOrigin.Current);
                }
                // end copy src to dst

                //Now draw a white square in the middle of the frame
                // Normally you'd do this code within the loop above but here it is in a separate loop for clarity;

                dstp.Seek((dst_height / 2 - SquareSize / 2) * dst_pitch, SeekOrigin.Begin);  // move pointer to SquareSize/2 lines from the middle of the frame;

                int woffset = dst_width / 8 - SquareSize / 2;  // lets precalulate the width offset like we do for the lines.

                buffer = BitConverter.GetBytes((int)0x00FFFFFF);// initialize the buffer with white pixel
                                                                // LSB = Blue, MSB = "spare" byte
                for (h = 0; h < SquareSize; h++) { // only scan SquareSize number of lines 
                    dstp.Seek(woffset * 4, SeekOrigin.Current);
                    for (w = 0; w < SquareSize; w += 1) { // only scans the middle SquareSize pixels of a line 
                        dstp.Write(buffer, 0, 4);

                    }
                    dstp.Seek(woffset * 4, SeekOrigin.Current);
                    dstp.Seek(dst_pitch - dst_width, SeekOrigin.Current);
                }
            }

            if (vi.IsYUY2()) {
                // This code deals with YUY2 colourspace where each 4 byte sequence represents
                // 2 pixels, (Y1, U, Y2 and then V).

                // This colourspace is more memory efficient than RGB32 but can be more awkward to use sometimes.
                // However, it can still be manipulated 32bits at a time depending on the
                // type of filter you are writing

                // There is no difference in code for this loop and the RGB32 code due to a coincidence :-)
                // 1) YUY2 frame_width is half of an RGB32 one
                // 2) But in YUY2 colourspace, a 32bit variable holds 2 pixels instead of the 1 in RGB32 colourspace.
                byte[] buffer = new byte[4];
                for (h = 0; h < src_height; h++) {       // Loop from top line to bottom line (opposite of RGB colourspace).
                    for (w = 0; w < src_width / 4; w += 1) {    // and from leftmost double-pixel to rightmost one. 
                        srcp.Read(buffer, 0, 4);
                        dstp.Write(buffer, 0, 4);// Copy 2 pixels worth of information from source to destination.
                    }                                                                                   // at a time by temporarily treating the src and dst pointers as
                                                                                                        // 32bit (4 byte) pointers intead of 8 bit (1 byte) pointers
                    srcp.Seek(src_pitch - src_width, SeekOrigin.Current); // Seek to next line.
                    dstp.Seek(dst_pitch - dst_width, SeekOrigin.Current);
                }
                // end copy src to dst

                //Now draw the other clip inside a square in the middle of the frame
                // Normally you'd do this code within the loop above but here it is in a separate loop for clarity;

                dstp.Seek((dst_height / 2 - SquareSize / 2) * dst_pitch, SeekOrigin.Begin);  // move pointer to SquareSize/2 lines from the middle of the frame;

                windowp = window.GetReadStream();

                int woffset = dst_width / 8 - SquareSize / 4;  // lets precalulate the width offset like we do for the lines.
                for (h = 0; h < SquareSize; h++) { // only scan SquareSize number of lines 
                    dstp.Seek(woffset * 4, SeekOrigin.Current);
                    for (w = 0; w < SquareSize / 2; w += 1) { // only scans the middle SquareSize pixels of a line 

                        windowp.Read(buffer, 0, 4);
                        dstp.Write(buffer, 0, 4);   // Pixels to come from top left of WindowVideo
                    }
                    windowp.Seek(window_pitch - SquareSize * 2, SeekOrigin.Current); // Seek to next line.
                    dstp.Seek(woffset * 4, SeekOrigin.Current);
                    dstp.Seek(dst_pitch - dst_width, SeekOrigin.Current);
                }
            }

            if (vi.IsYV12()) {

                // This code deals with YV12 colourspace where the Y, U and V information are
                // stored in completely separate memory areas

                // This colourspace is the most memory efficient but usually requires 3 separate loops
                // However, it can actually be easier to deal with than YUY2 depending on your filter algorithim

                // So first of all deal with the Y Plane

                for (h = 0; h < src_height; h++) {       // Loop from top line to bottom line (Sames as YUY2.
                    for (w = 0; w < src_width; w++)       // Loop from left side of the image to the right side.
                        dstp.WriteByte((byte)srcp.ReadByte());            // Copy each byte from source to destination.									

                    srcp.Seek(src_pitch - src_width, SeekOrigin.Current); // Seek to next line.
                    dstp.Seek(dst_pitch - dst_width, SeekOrigin.Current);
                }
                // end copy Y Plane src to dst

                //Now set the Y plane bytes to maximum in the middle of the frame
                // Normally you'd do this code within the loop above but here it is in a separate loop for clarity;

                dstp.Seek((dst_height / 2 - SquareSize / 2) * dst_pitch, SeekOrigin.Begin);  // move pointer to SquareSize/2 lines from the middle of the frame;

                int woffset = dst_width / 2 - SquareSize / 2;  // lets precalulate the width offset like we do for the lines.

                for (h = 0; h < SquareSize; h++) { // only scan SquareSize number of lines 
                    dstp.Seek(woffset, SeekOrigin.Current);
                    for (w = 0; w < SquareSize; w += 1) { // only scans the middle SquareSize pixels of a line
                        dstp.WriteByte(235);        // Set Y values to maximum
                    }
                    dstp.Seek(woffset, SeekOrigin.Current);
                    dstp.Seek(dst_pitch - dst_width, SeekOrigin.Current);
                }
                // end of Y plane Code

                // This section of code deals with the U and V planes of planar formats (e.g. YV12)
                // So first of all we have to get the additional info on the U and V planes

                int dst_pitchUV = dst.GetPitch(YUVPlanes.PLANAR_U); // The pitch,height and width information
                int dst_widthUV = dst.GetRowSize(YUVPlanes.PLANAR_U);   // is guaranted to be the same for both
                int dst_heightUV = dst.GetHeight(YUVPlanes.PLANAR_U);   // the U and V planes so we only the U
                int src_pitchUV = src.GetPitch(YUVPlanes.PLANAR_U); // plane values and use them for V as
                int src_widthUV = src.GetRowSize(YUVPlanes.PLANAR_U);   // well
                int src_heightUV = src.GetHeight(YUVPlanes.PLANAR_U);   //

                //Copy U plane src to dst
                srcp = src.GetReadStream(YUVPlanes.PLANAR_U);
                dstp = dst.GetWriteStream(YUVPlanes.PLANAR_U);

                for (h = 0; h < src_heightUV; h++) {
                    for (w = 0; w < src_widthUV; w++)
                        dstp.WriteByte((byte)srcp.ReadByte());
                    ;
                    srcp.Seek(src_pitchUV - src_widthUV, SeekOrigin.Current);
                    dstp.Seek(dst_pitchUV - dst_widthUV, SeekOrigin.Current);
                }
                // end copy U plane src to dst

                //Now set the U plane bytes to no colour in the middle of the frame
                // Normally you'd do this code within the loop above but here it is in a separate loop for clarity;

                // reset the destination pointer to the top, left pixel.
                dstp.Seek((dst_heightUV / 2 - SquareSize / 4) * dst_pitchUV, SeekOrigin.Begin);  // note change in how much we dived SquareSize by
                                                                                                 // as the U plane height is half the Y plane

                woffset = dst_widthUV / 2 - SquareSize / 4;  // And the divisor changes here as well compared to Y plane code.

                for (h = 0; h < SquareSize / 2; h++) { // only scan SquareSize/2 number of lines (because the U plane height is half the Y)
                    dstp.Seek(woffset, SeekOrigin.Current);
                    for (w = 0; w < SquareSize / 2; w += 1) { // only scans the middle SquareSize/2 bytes of a line because ... U=Y/2 :-)
                        dstp.WriteByte(128);        // Set U Value to no colour
                    }
                    dstp.Seek(woffset, SeekOrigin.Current);
                    dstp.Seek(dst_pitchUV - dst_widthUV, SeekOrigin.Current);
                }
                // end of U plane Code



                //Copy V plane src to dst
                srcp = src.GetReadStream(YUVPlanes.PLANAR_V);
                dstp = dst.GetWriteStream(YUVPlanes.PLANAR_V);

                for (h = 0; h < src_heightUV; h++) {
                    for (w = 0; w < src_widthUV; w++)
                        dstp.WriteByte((byte)srcp.ReadByte());
                    ;
                    srcp.Seek(src_pitchUV - src_widthUV, SeekOrigin.Current);
                    dstp.Seek(dst_pitchUV - dst_widthUV, SeekOrigin.Current);
                }
                // end copy V plane src to dst

                //Now set the V plane bytes to no colour in the middle of the frame
                // the code is identical to the code for U plane apart from getting the frame start pointer.
                // Normally you'd do this code within the loop above but here it is in a separate loop for clarity;

                dstp.Seek((dst_heightUV / 2 - SquareSize / 4) * dst_pitchUV, SeekOrigin.Begin);  // note change in how much we dived SquareSize by
                                                                                                 // as the V plane height is half the Y plane

                woffset = dst_widthUV / 2 - SquareSize / 4;  // And the divisor changes here as well compared to Y plane code.

                for (h = 0; h < SquareSize / 2; h++) { // only scan SquareSize/2 number of lines (because the V plane height is half the Y)
                    dstp.Seek(woffset, SeekOrigin.Current);
                    for (w = 0; w < SquareSize / 2; w += 1) { // only scans the middle SquareSize/2 bytes of a line because ... U=Y/2 :-)
                        dstp.WriteByte(128);        // Set V Value to no colour
                    }
                    dstp.Seek(woffset, SeekOrigin.Current);
                    dstp.Seek(dst_pitchUV - dst_widthUV, SeekOrigin.Current);
                }
                // end of U plane Code

            }

            //Release all VideoFrames except for the destination frame.
            //Important! If you don't do that, your memory will quickly run out.
            src.Dispose();
            window.Dispose();

            // As we now are finished processing the image, we return the destination image.
            return dst;

        }
    }
}
