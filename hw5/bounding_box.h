#ifndef HW_BOUNDING_BOX_H
#define HW_BOUNDING_BOX_H

#include "raytra.h"
#include "ray.h"
#include <limits>
#include <vector>
#include "assert.h"

enum class Axis { X, Y, Z };

Axis get_next_direction(Axis dir);

class BoundingBox {
public:
    static BoundingBox* combine(
            std::vector<BoundingBox*>::iterator first,
            std::vector<BoundingBox*>::iterator last
    );

    BoundingBox(float x_min, float x_max,
                float y_min, float y_max,
                float z_min, float z_max);

    virtual ~BoundingBox() {};
    Raytra::point center;
    int get_surface_index();
    void set_surface_index(int idx);
    float get_intersection_point(const Ray &ray) const;

    vec get_normal(const point& p) const;

    static bool box_compare_along_dir (
            const BoundingBox* a,
            const BoundingBox* b,
            Axis direction
    );
    float xmin, xmax, ymin, ymax, zmax, zmin;
    int id;
private:
    int surface_index;
};


#endif //HW_BOUNDING_BOX_H
