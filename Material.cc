#include "Material.h"

Material::Material(float dr, float dg, float db,
                   float sr, float sg, float sb,
                   float ir, float ig, float ib, float r)
{
    diffuse = {.red = dr, .green = dg, .blue = db};
    specular = {.red = sr, .green = sg, .blue = sb};
    ideal_specular = {.red = ir, .green = ig, .blue = ib};
    phong = r;
}
