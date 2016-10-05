#ifndef HW1_EXR_H
#define HW1_EXR_H

#include <ImfRgbaFile.h>
#include <ImfStringAttribute.h>
#include <ImfMatrixAttribute.h>
#include <ImfArray.h>

using namespace Imf;
using namespace Imath;

/* helper methods for the OpenEXR library */
class exr {
public:
    static void writeRgba (const char fileName[], const Rgba *pixels,
                           int width, int height);



};


#endif //HW1_EXR_H
