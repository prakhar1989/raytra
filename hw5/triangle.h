#ifndef HW_TRIANGLE_H
#define HW_TRIANGLE_H

#include "surface.h"

class Triangle: public Surface {
public:
    Triangle(float ax, float ay, float az,
             float bx, float by, float bz,
             float cx, float cy, float cz);
    virtual ~Triangle() { delete box; }
    void print();
    vec get_normal(const point& p) const;
    float get_intersection_point(const Ray& ray);
    bool is_front_facing(const Ray& ray) const;
    BoundingBox* get_bounding_box() const;

private:
    point p1, p2, p3;
    vec normal;
    BoundingBox* box;
};


#endif //HW_TRIANGLE_H
