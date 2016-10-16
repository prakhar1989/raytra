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

/* as derived using Cramer's rule */
float Triangle::get_intersection_point(const Ray &ray)
{
    float a = p1.x - p2.x; float b = p1.y - p2.y; float c = p1.z - p2.z;
    float d = p1.x - p3.x; float e = p1.y - p3.y; float f = p1.z - p3.z;
    float g = ray.dir.x; float h = ray.dir.y; float i = ray.dir.z;
    float j = p1.x - ray.origin.x; float k = p1.y - ray.origin.y;
    float l = p1.z - ray.origin.z;

    float akjb = a * k - j * b;
    float jcal = j * c - a * l;
    float blkc = b * l - k * c;
    float eihf = e * i - f * h;
    float gfdi = g * f - d * i;
    float dheg = d * h - e * g;

    float M = a * eihf + b * gfdi + c * dheg;

    float t = - (f * akjb + e * jcal + d * blkc) / M;
    if (t < 0)
        return -1;

    float beta =  (j * eihf + k * gfdi + l * dheg) / M;
    if (beta < 0 || beta > 1)
        return -1;

    float gamma = (i * akjb + h * jcal + g * blkc) / M;
    if (gamma < 0 || 1 - beta < gamma)
        return -1;

    return t;
}


