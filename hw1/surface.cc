#include "surface.h"
#include <iostream>


Sphere::Sphere(const Point &center, float radius) : center(center), radius(radius) {}
Sphere::Sphere(float x, float y, float z, float r) {
    Point c;
    c.x = x; c.y = y; c.z = z; center = c;
    radius = r;
}
void Sphere::print() {
    printf("Sphere((%f, %f, %f), %f)\n", center.x, center.y, center.z, radius);
}
