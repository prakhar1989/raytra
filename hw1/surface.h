#ifndef HW1_SURFACE_H
#define HW1_SURFACE_H

#include "Material.h"

class Surface {
public:
    virtual void print() = 0;
    Material* material = nullptr;
};

class Sphere: public Surface {
public:
    Sphere(float x, float y, float z, float r);
    Sphere(const Raytra::Point &center, float radius);
    void print();

    Raytra::Point center;
    float radius;
};

#endif //HW1_SURFACE_H
