#ifndef HW_SURFACE_H
#define HW_SURFACE_H

#include "Material.h"
#include "ray.h"

class Surface {
public:
    virtual void print() = 0;
    virtual float get_intersection_point(const Ray& ray) = 0;
    virtual ~Surface() {};
    Material* material = nullptr;
};

class Sphere: public Surface {
public:
    Sphere(float x, float y, float z, float r);

    virtual ~Sphere() { delete material; }

    void print();
    float get_intersection_point(const Ray& ray);

    Raytra::point center;
    float radius;
};

#endif //HW_SURFACE_H
