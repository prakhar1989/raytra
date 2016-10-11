#ifndef HW_SURFACE_H
#define HW_SURFACE_H

#include "Material.h"
#include "ray.h"

class Surface {
public:
    virtual void print() = 0;
    virtual float get_intersection_point(const Ray& ray) = 0;
    virtual vec get_normal(const point& p) const = 0;

    virtual ~Surface() {};
    std::shared_ptr<Material> material;
};

#endif //HW_SURFACE_H
