#ifndef HW_SPHERE_H
#define HW_SPHERE_H

#include "surface.h"

class Sphere: public Surface {
public:
    Sphere(float x, float y, float z, float r);

    virtual ~Sphere() { delete material; }

    void print();
    float get_intersection_point(const Ray& ray);

    Raytra::point center;
    float radius;
};


#endif //HW_SPHERE_H
