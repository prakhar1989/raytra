#include "bounding_box.h"

/**
 * Combines two bounding boxes to return a new bounding box
 * that encompasses both of them.
 */
BoundingBox* BoundingBox::combine (
    const std::vector<BoundingBox *>::iterator first,
    const std::vector<BoundingBox *>::iterator last
)
{
    float x_min = std::numeric_limits<float>::infinity();
    float y_min = std::numeric_limits<float>::infinity();
    float z_min = std::numeric_limits<float>::infinity();

    float x_max = -std::numeric_limits<float>::infinity();
    float y_max = -std::numeric_limits<float>::infinity();
    float z_max = -std::numeric_limits<float>::infinity();

    for (auto p = first; p != last; ++p) {
        BoundingBox *&box = *p;
        x_min = fminf(x_min, box->xmin);
        y_min = fminf(y_min, box->ymin);
        z_min = fminf(z_min, box->zmin);

        x_max = fmaxf(x_max, box->xmax);
        y_max = fmaxf(y_max, box->ymax);
        z_max = fmaxf(z_max, box->zmax);
    }

    return new BoundingBox(x_min, x_max, y_min, y_max, z_min, z_max);
}

/**
 * Compares two bounding boxes along an axis and returns
 * true if the former lies further away from the origin than
 * the latter
 */
bool BoundingBox::compare_along_axis(
        const BoundingBox *a,
        const BoundingBox *b,
        Axis axis
)
{
    if (axis == Axis::X)
        return a->center.x > b->center.x;
    if (axis == Axis::Y)
         return a->center.y > b->center.y;
    return a->center.z > b->center.z;
}

/**
 * Given a direction, returns the next direction
 * Useful for cycling through the directions
 */
Axis next_axis(Axis axis)
{
    if (axis == Axis::X)
        return Axis::Y;
    if (axis == Axis::Y)
        return Axis::Z;
    return Axis::X;
}

/**
 * Returns a new bounding box that has a gauranteed thickness
 * and that is unassigned to a surface
 */
BoundingBox::BoundingBox(float x_min, float x_max,
                         float y_min, float y_max,
                         float z_min, float z_max)
{
    xmin = x_min; xmax = x_max; ymin = y_min;
    ymax = y_max; zmin = z_min; zmax = z_max;

    /* give a thickness to the bounding box */
    if (xmax == xmin)
        xmax += 0.01;

    if (ymax == ymin)
        ymax += 0.01;

    if (zmax == zmin)
        zmax += 0.01;

    center = {
            .x = (xmin + xmax) / 2,
            .y = (ymin + ymax) / 2,
            .z = (zmin + zmax) / 2
    };

    /* at init, do not assign it to any surface */
    surface_index = -1;

    /* set a random id to the box, useful for debugging */
    id = std::rand() % 100;
}

/**
 * Returns the normal at the point
 */
vec BoundingBox::get_normal(const point& p) const
{
    if (approx_equal(p.x, xmin))
        return { -1, 0, 0 };

    if (approx_equal(p.x, xmax))
        return { 1, 0, 0 };

    if (approx_equal(p.y, ymin))
        return { 0, -1, 0 };

    if (approx_equal(p.y, ymax))
        return { 0, 1, 0 };

    if (approx_equal(p.z, zmax))
        return { 0, 0, 1};

    if (approx_equal(p.z, zmin))
        return { 0, 0, -1};

    /* ideally: this should not be reachable */
    return { -1, -1, -1 };
}

/**
 * Returns the parameter at the intersection point of a bounding box
 * with a incident ray if it intersects or -1 otherwise
 */
float BoundingBox::get_intersection_point(const Ray &ray) const
{
    float hit_min = 0;
    float hit_max = std::numeric_limits<float>::infinity();

    /* for X */
    if ((ray.origin.x < xmin || ray.origin.x > xmax) && ray.dir.x == 0)
        return -1;

    float tx_min = (xmin - ray.origin.x) / ray.dir.x;
    float tx_max = (xmax - ray.origin.x) / ray.dir.x;

    if (ray.dir.x < 0)
        std::swap(tx_min, tx_max);

    if (tx_min > hit_min) hit_min = tx_min;
    if (tx_max < hit_max) hit_max = tx_max;
    if (hit_min > hit_max) return -1;

    /* for Y */
    if (ray.dir.y == 0 && (ray.origin.y < ymin || ray.origin.y > ymax))
        return -1;

    float ty_min = (ymin - ray.origin.y) / ray.dir.y;
    float ty_max = (ymax - ray.origin.y) / ray.dir.y;

    if (ray.dir.y < 0) std::swap(ty_min, ty_max);
    if (ty_min > hit_min) hit_min = ty_min;
    if (ty_max < hit_max) hit_max = ty_max;
    if (hit_min > hit_max) return -1;

    /* for Z */
    if (ray.dir.z == 0 && (ray.origin.z < zmin || ray.origin.z > zmax))
        return -1;

    float tz_min = (zmin - ray.origin.z) / ray.dir.z;
    float tz_max = (zmax - ray.origin.z) / ray.dir.z;

    if (ray.dir.z < 0)
        std::swap(tz_min, tz_max);

    if (tz_min > hit_min) hit_min = tz_min;
    if (tz_max < hit_max) hit_max = tz_max;

    if (hit_min > hit_max) return -1;
    return hit_min;
}

/**
 * Prints the id and the center of the bounding box.
 * Useful for debugging.
 */
void BoundingBox::print()
{
    std::cout << "id: " << id << ", " << center << std::endl;
}
