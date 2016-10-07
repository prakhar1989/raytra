#ifndef HW_MATERIAL_H
#define HW_MATERIAL_H

#include "raytra.h"

class Material {
public:
    Material(float dr, float dg, float db,
             float sr, float sg, float sb,
             float ir, float ig, float ib, float r);
    Raytra::color diffuse;
    Raytra::color specular;
    Raytra::color ideal_specular;
    float phong;
};

#endif //HW_MATERIAL_H
