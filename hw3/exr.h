#ifndef HW_EXR_H
#define HW_EXR_H

#include <ImfRgbaFile.h>
#include <ImfStringAttribute.h>
#include <ImfMatrixAttribute.h>
#include <ImfArray.h>

using namespace Imf;
using namespace Imath;

/* helper methods for the OpenEXR library */
class exr {
public:
    static void writeRgba (const char fileName[], const Rgba *pixels, int width, int height);
};

#endif //HW_EXR_H
