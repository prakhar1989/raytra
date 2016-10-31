#ifndef HW_SPHERE_H
#define HW_SPHERE_H

#include "surface.h"

class Sphere: public Surface {
public:
    Sphere(float x, float y, float z, float r);
    virtual ~Sphere() {}
    void print();
    float get_intersection_point(const Ray& ray);
    vec get_normal(const point& p) const;
    bool is_front_facing(const Ray& ray) const { return true; }
    BoundingBox* get_bounding_box() const;

private:
    Raytra::point center;
    float radius;
    BoundingBox* box;
};


#endif //HW_SPHERE_H
