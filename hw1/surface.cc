#include "surface.h"
#include <iostream>

Sphere::Sphere(const Raytra::point &center, float radius) : center(center), radius(radius) {}

Sphere::Sphere(float x, float y, float z, float r) {
    center = { .x = x, .y = y, .z = z };
    radius = r;
}

void Sphere::print() {
    printf("Sphere((%f, %f, %f), %f)\n", center.x, center.y, center.z, radius);
}

float Sphere::get_intersection_point(const Ray &ray) {
    Raytra::vector v = ray.origin - center;

    float d2 = dot(ray.dir, ray.dir);
    float disc = powf(dot(ray.dir, v), 2) - d2 * (dot(v, v) - (radius * radius));

    if (disc < 0)
        return -1; // no intersection

    float s = dot(-ray.dir, v);

    // if disc is ~ 0
    if (disc < 10e-4)
        return s/d2;

    // if disc is non-zero and positive
    float disc1 = (s + sqrtf(disc)) / d2;
    float disc2 = (s - sqrtf(disc)) / d2;

    return std::min(disc1, disc2);
}
