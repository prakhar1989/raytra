#ifndef HW6_AREALIGHT_H
#define HW6_AREALIGHT_H

#include "raytra.h"

using namespace Raytra;

class AreaLight {
public:
    AreaLight() {};
    ~AreaLight() {}
    AreaLight (
        float x, float y, float z,      // position
        float nx, float ny, float nz,   // normal
        float ux, float uy, float uz,   // u-axis
        float len,                       // len of each side
        float r, float g, float b       // exitance
    );

private:
    color c;
    point position;
    float side_length;
    vec normal;
    vec U;
    vec V;
};


#endif //HW_AREALIGHT_H
