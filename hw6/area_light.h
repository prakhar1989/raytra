#ifndef HW6_AREALIGHT_H
#define HW6_AREALIGHT_H

#include "raytra.h"
#include <vector>
#include "surface.h"
#include "BVHTree.h"

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

    Raytra::point get_point(float i, float j, unsigned int strata_count);

    bool is_occluded_by (
        const Raytra::point &point_on_light,
        const Raytra::point &intersection_point,
        const std::vector<Surface*> &surfaces,
        const BVHTree *tree
    );

private:
    color c;
    Raytra::point position;
    float side_length;
    vec normal;
    vec U;
    vec V;
};


#endif //HW_AREALIGHT_H
