#include "triangle.h"

Triangle::Triangle(float ax, float ay, float az,
                   float bx, float by, float bz,
                   float cx, float cy, float cz)
{
    p1 = {.x = ax, .y = ay, .z = az};
    p2 = {.x = bx, .y = by, .z = bz};
    p3 = {.x = cx, .y = cy, .z = cz};

    normal = norm(cross(p2 - p1, p3 - p2));
}

void Triangle::print()
{
    std::cout << "Triangle: p1 " << p1
              << ", p2 " << p2
              << ", p3 " << p3
              << std::endl;
}

vec Triangle::get_normal(const point& p) const {
    return normal;
}

float Triangle::get_intersection_point(const Ray &ray)
{
    return 0;
}


