#include "bounding_box.h"


BoundingBox::BoundingBox(float x_min, float x_max,
                         float y_min, float y_max,
                         float z_min, float z_max)
{
    xmin = x_min; xmax = x_max; ymin = y_min; ymax = y_max;
    zmin = z_min; zmax = z_max;

    center = {
            .x = (xmin + xmax) / 2,
            .y = (ymin + ymax) / 2,
            .z = (zmin + zmax) / 2
    };

    // -1 implies not assigned to a surface
    surface_index = -1;
}

int BoundingBox::get_surface_index()
{
    return surface_index;
}

void BoundingBox::set_surface_index(int idx)
{
    surface_index = idx;
}
