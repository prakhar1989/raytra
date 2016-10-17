#include "point_light.h"

PointLight::PointLight(float x, float y, float z, float r, float g, float b)
{
    c = {.red = r, .green = g, .blue = b};
    position = {.x = x, .y = y, .z = z};
    intensity = 1;
}
