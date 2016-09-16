#include "hw_zero.h"
#include <fstream>


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


int getQuadrant(int x, int y, int w, int h)
{
    // either 0 or 2
    if (x <= 0.33 * w) {
        if (y < 0.5 * h)
            return 0;
        return 1;
    }
    // 2 or 3
    if (y < 0.5 * h)
        return 2;
    return 3;
}

bool doesFileExist(const char* filename)
{
    std::ifstream infile(filename);
    return infile.good();
}


int main (int argc, char *argv[])
{
    try
    {
        if (!doesFileExist(argv[1])) {
            cout << "File doesn't exist" << endl;
            return 1;
        }

        cout << "reading " << argv[1] << endl;
        int w, h;
        Array2D<Rgba> p;

        readRgba (argv[1], p, w, h);
        cout << "image width height is: " << w << "  " << h << endl;

        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {

                Rgba &px = p[y][x];  // get the pixel we are interested in

                // 0 -> Red, 1 -> Blue, 2 -> Green, 3 -> luminance
                switch(getQuadrant(x, y, w, h)) {
                    case 0: px.g = 0; px.b = 0; break;
                    case 1: px.r = 0; px.g = 0; break;
                    case 2: px.r = 0; px.b = 0; break;
                    case 3: {
                                double l = 0.2126 * px.r + 0.7152 * px.g + 0.0722 * px.b;
                                px.r = l; px.g = l; px.b = l; break;
                            }
                }
            }
        }

        cout << "writing output image" << endl;

        writeRgba ("hw0.exr", &p[0][0], w, h);

        cout << "done." << endl;
    }

    catch (const std::exception &exc) {
        std::cerr << exc.what() << std::endl;
        return 1;
    }

    return 0;
}
