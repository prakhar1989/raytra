#ifndef HW_POINT_LIGHT_H
#define HW_POINT_LIGHT_H

#include "raytra.h"
#include <vector>
#include "ray.h"
#include "surface.h"

using namespace Raytra;

class PointLight {
public:
    PointLight() {};
    PointLight(float x, float y, float z, float r, float g, float b);
    color compute_shading (
            const Surface* surface,
            const Ray& ray,
            const Raytra::point& point
    );

    bool is_occluded_by (
            const Raytra::point& point,
            const std::vector<Surface*>& surfaces
    );

private:
    color diffuse_shading (
            const Surface* surface,
            const Ray& ray,
            const Raytra::point& point
    );

    color specular_shading (
            const Surface* surface,
            const Ray& ray,
            const Raytra::point& point
    );

    point position;
    color c;
    float intensity;
};

#endif //HW_POINT_LIGHT_H
