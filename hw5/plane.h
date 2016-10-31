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
    bool is_front_facing(const Ray& ray) const;
    BoundingBox* get_bounding_box() const { return box; }

private:
    vec normal;
    float dist;
    BoundingBox* box = nullptr;
};


#endif //HW_PLANE_H
