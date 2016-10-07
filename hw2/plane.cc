#include "plane.h"

Plane::Plane(float x, float y, float z, float d)
{
    dist = d;
    normal = norm((vec) {.x = x, .y = y, .z = z});
}

void Plane::print()
{
    std::cout << "Plane: N = " << normal << ", dist: " << dist << std::endl;
}

float Plane::get_intersection_point(const Ray &ray)
{
    return 0;
}
