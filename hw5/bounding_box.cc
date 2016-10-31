#include "bounding_box.h"

bool BoundingBox::box_compare_along_dir (
        const BoundingBox* a,
        const BoundingBox* b,
        SortDirection direction
)
{
    switch(direction)
    {
        case SortDirection::X : return a->center.x > b->center.x;
        case SortDirection::Y : return a->center.y > b->center.y;
        case SortDirection::Z : return a->center.z > b->center.z;
    }
}

SortDirection get_next_direction(SortDirection dir)
{
    switch(dir)
    {
        case SortDirection::X  : return SortDirection::Y;
        case SortDirection::Y  : return SortDirection::Z;
        case SortDirection::Z  : return SortDirection::X;
    }
}

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

bool BoundingBox::does_intersect(const Ray &ray) const
{
    float hit_min = 0;
    float hit_max = std::numeric_limits<float>::infinity();

    /* for X */
    if (ray.dir.x == 0 && (ray.origin.x < xmin || ray.origin.x > xmax))
        return false;

    float tx_min = (xmin - ray.origin.x) / ray.dir.x;
    float tx_max = (xmax - ray.origin.x) / ray.dir.x;

    if (ray.dir.x < 0)
        std::swap(tx_min, tx_max);

    if (tx_min > hit_min) hit_min = tx_min;
    if (tx_max < hit_max) hit_max = tx_max;
    if (hit_min > hit_max) return false;

    /* for Y */
    if (ray.dir.y == 0 && (ray.origin.y < ymin || ray.origin.y > ymax))
        return false;

    float ty_min = (ymin - ray.origin.y) / ray.dir.y;
    float ty_max = (ymax - ray.origin.y) / ray.dir.y;

    if (ray.dir.y < 0) std::swap(ty_min, ty_max);
    if (ty_min > hit_min) hit_min = ty_min;
    if (ty_max < hit_max) hit_max = ty_max;
    if (hit_min > hit_max) return false;

    /* for Z */
    if (ray.dir.z == 0 && (ray.origin.z < zmin || ray.origin.z > zmax))
        return false;

    float tz_min = (zmin - ray.origin.z) / ray.dir.z;
    float tz_max = (zmax - ray.origin.z) / ray.dir.z;

    if (ray.dir.z < 0)
        std::swap(tz_min, tz_max);

    if (tz_min > hit_min) hit_min = tz_min;
    if (tz_max < hit_max) hit_max = tz_max;
    return hit_min < hit_max;
}
