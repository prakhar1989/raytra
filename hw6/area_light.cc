#include "area_light.h"

AreaLight::AreaLight (
        float x, float y, float z,      // position
        float nx, float ny, float nz,   // normal
        float ux, float uy, float uz,   // u-axis
        float len,                       // len of each side
        float r, float g, float b       // exitance
)
{
    c = {.red = r, .blue = b, .green = g};
    side_length = len;
    position = {x, y, z};
    normal = {nx, ny, nz};
    U = {ux, uy, uz};
    V = norm(cross(U, normal));
}
