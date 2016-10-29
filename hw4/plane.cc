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
   if (dot(ray.dir, normal) == 0)
        return -1; // ray and plane are parallel

    float s = ray.origin.x * normal.x + ray.origin.y * normal.y
              + ray.origin.z * normal.z;

    return -(s - dist) / dot(ray.dir, normal);
}

vec Plane::get_normal(const point &p) const
{
    return normal;
}

bool Plane::is_front_facing(const Ray &ray) const
{
    return dot(normal, ray.dir) < 0;
}
