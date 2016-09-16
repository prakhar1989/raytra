#include "exr_test.h"


void writeRgba (const char fileName[], const Rgba *pixels, int width, int height)
{
    //
    // Write an RGBA image using class RgbaOutputFile.
    //
    //	- open the file
    //	- describe the memory layout of the pixels
    //	- store the pixels in the file
    //
    RgbaOutputFile file (fileName, width, height, WRITE_RGBA);
    file.setFrameBuffer (pixels, 1, width);
    file.writePixels (height);
}



void readRgba (const char fileName[], Array2D<Rgba> &pixels, int &width, int &height)
{
    //
    // Read an RGBA image using class RgbaInputFile:
    //
    //	- open the file
    //	- allocate memory for the pixels
    //	- describe the memory layout of the pixels
    //	- read the pixels from the file
    //
    RgbaInputFile file (fileName);
    Box2i dw = file.dataWindow();
    width  = dw.max.x - dw.min.x + 1;
    height = dw.max.y - dw.min.y + 1;
    pixels.resizeErase (height, width);
    file.setFrameBuffer (&pixels[0][0] - dw.min.x - dw.min.y * width, 1, width);
    file.readPixels (dw.min.y, dw.max.y);
}



int main (int argc, char *argv[])
{
    try
    {
        cout << "reading " << argv[1] << endl;
        int w, h;
        Array2D<Rgba> p;

        // you should test for file existence before this!
        readRgba (argv[1], p, w, h);
        cout << "image width height is: " << w << "  " << h << endl;

        // move everything to red channel, this will give an intensity image
        // in red only:
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {

                Rgba &px = p[y][x];  // get the pixel we are interested in

                // we're going to put all the channels into one. We'll also
                // normalize a bit - which is not really necessary in an exr image
                // since it has the high range. But if this was an image where each
                // pixel is only allowed to range [0..1] (floating point) or [0..255]
                // (integer) this normalization will keep everything in range.
                //
                px.r += (px.r + px.b + px.g) / 3.0;
                // zero everything except the red channel
                px.g = 0;
                px.b = 0;
                px.a = 1;
            }
        }

        cout << "writing output image" << endl;

        writeRgba ("allred.exr", &p[0][0], w, h);

        cout << "done." << endl;
    }

    catch (const std::exception &exc) {
        std::cerr << exc.what() << std::endl;
        return 1;
    }

    return 0;
}
