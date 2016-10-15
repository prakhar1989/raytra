#include "point_light.h"

Point_light::Point_light(float x, float y, float z, float r, float g, float b)
{
    c = {.red = r, .green = g, .blue = b};
    position = {.x = x, .y = y, .z = z};
    intensity = 1;
}

