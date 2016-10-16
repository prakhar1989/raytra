#ifndef HW_POINT_LIGHT_H
#define HW_POINT_LIGHT_H

#include "raytra.h"
using namespace Raytra;

class PointLight {
public:
    PointLight() {};
    PointLight(float x, float y, float z, float r, float g, float b);

    point position;
    color c;
    float intensity;
};

#endif //HW_POINT_LIGHT_H
