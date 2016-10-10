#ifndef HW_POINT_LIGHT_H
#define HW_POINT_LIGHT_H

#include "raytra.h"
using namespace Raytra;

class Point_light {
public:
    Point_light() {};
    Point_light(float x, float y, float z, float r, float g, float b);

    point position;
    color c;
    float intensity;
};

#endif //HW_POINT_LIGHT_H
