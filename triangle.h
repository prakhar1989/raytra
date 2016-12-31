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
    vec get_normal() const { return normal; }

    float get_intersection_point(const Ray& ray);
    bool is_front_facing(const Ray& ray) const;
    BoundingBox* get_bounding_box() const;

    void set_vertex_normals (
        const vec& v1, const vec& v2, const vec& v3
    );

private:
    point p1, p2, p3;
    vec normal;  // planar normal
    vec n1, n2, n3; // vertix normals
    bool in_mesh = false;
    BoundingBox* box;
};


#endif //HW_TRIANGLE_H
