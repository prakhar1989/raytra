#ifndef HW_BOUNDING_BOX_H
#define HW_BOUNDING_BOX_H

#include "raytra.h"
#include "ray.h"
#include <limits>

enum SortDirection { X, Y, Z };

SortDirection get_next_direction(SortDirection dir);

class BoundingBox {
public:
    BoundingBox(float x_min, float x_max,
                float y_min, float y_max,
                float z_min, float z_max);
    Raytra::point center;
    int get_surface_index();
    void set_surface_index(int idx);
    bool does_intersect(const Ray& ray) const;
private:
    float xmin, xmax, ymin, ymax, zmax, zmin;
    int surface_index;
};


#endif //HW_BOUNDING_BOX_H
