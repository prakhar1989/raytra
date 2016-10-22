#ifndef HW_TRIANGLE_H
#define HW_TRIANGLE_H

#include "surface.h"

class Triangle: public Surface {
public:
    Triangle(float ax, float ay, float az,
             float bx, float by, float bz,
             float cx, float cy, float cz);
    virtual ~Triangle() {}
    void print();
    vec get_normal(const point& p) const;
    float get_intersection_point(const Ray& ray);
    bool is_one_sided() const { return true; }
private:
    point p1, p2, p3;
    vec normal;
};


#endif //HW_TRIANGLE_H
