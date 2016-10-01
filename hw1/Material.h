#ifndef HW1_MATERIAL_H
#define HW1_MATERIAL_H

#include <printf.h>
#include <iosfwd>

using namespace std;

struct color {
    float red, green, blue;
};

class Material {
public:
    Material(float dr, float dg, float db,
             float sr, float sg, float sb,
             float ir, float ig, float ib, float r);
    color diffuse;
    color specular;
    color ideal_specular;
    float phong;
};

#endif //HW1_MATERIAL_H
