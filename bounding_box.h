#ifndef HW_BOUNDING_BOX_H
#define HW_BOUNDING_BOX_H

#include "raytra.h"
#include "ray.h"
#include <limits>
#include <vector>

enum class Axis { X, Y, Z };
Axis next_axis(Axis dir);

class BoundingBox {
public:
    static BoundingBox* combine (
            const std::vector<BoundingBox*>::iterator first,
            const std::vector<BoundingBox*>::iterator last
    );

    static bool compare_along_axis (
            const BoundingBox*, const BoundingBox*, Axis
    );

    BoundingBox(float x_min, float x_max,
                float y_min, float y_max,
                float z_min, float z_max);

    virtual ~BoundingBox() {};

    /* getter/setter for surface index */
    int get_surface_index() { return surface_index; }
    void set_surface_index(int idx) { surface_index = idx; }

    float get_intersection_point(const Ray &ray) const;
    vec get_normal(const point& p) const;

    void print();
private:
    float xmin, xmax, ymin, ymax, zmax, zmin;
    int id;
    Raytra::point center;
    int surface_index;
};


#endif //HW_BOUNDING_BOX_H
