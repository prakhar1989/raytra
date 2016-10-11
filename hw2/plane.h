#ifndef HW_PLANE_H
#define HW_PLANE_H

#include "surface.h"

class Plane: public Surface {
public:
    Plane(float x, float y, float z, float d);
    virtual ~Plane() {}
    void print();
    float get_intersection_point(const Ray& ray);
    vec get_normal(const point& p) const;

    // distance between origin and plane in direction of the normal
    vec normal;
    float dist;
};


#endif //HW_PLANE_H
