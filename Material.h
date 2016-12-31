#ifndef HW_MATERIAL_H
#define HW_MATERIAL_H

#include "raytra.h"

using namespace Raytra;

class Material {
public:
    virtual ~Material() {};

    inline bool is_reflective() {
        return !(ideal_specular.red == 0 &&
               ideal_specular.green == 0 &&
               ideal_specular.blue == 0);
    }

    Material(float dr, float dg, float db,
             float sr, float sg, float sb,
             float ir, float ig, float ib, float r);

    color diffuse; color specular; color ideal_specular;

    float phong;
};

#endif //HW_MATERIAL_H
