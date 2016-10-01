#ifndef HW1_SPHERE_H
#define HW1_SPHERE_H

#include <iosfwd>
#include "surface.h"

class Sphere: Surface {
public:
    Sphere(float x, float y, float z, float r);
    Sphere(const Point &center, float radius);
    void print();

    Point center;
    float radius;
};


#endif //HW1_SPHERE_H
