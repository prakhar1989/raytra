#ifndef HW_RAY_H
#define HW_RAY_H
#include <assert.h>

using namespace Raytra;

class Ray {
public:
    Ray(const point &origin, const vec &direction) : origin(origin), dir(direction) {}
    point get_point(float t) const;
    float offset(point p) const;

    point origin;
    vec dir;
};

/**
 * Returns a point on the ray
 *
 * @param t - the parameter on the ray
 * @return  a point on the ray
 */
inline point Ray::get_point(float t) const
{
    return t * dir + origin;
}

/**
 * Returns an offset at the point on the ray
 *
 * @param p - a point on the ray
 * @return a param t on the ray at the intersection
 */
inline float Ray::offset(point p) const
{
    assert(!(dir.x == 0 && dir.y == 0 && dir.z == 0));

    if (dir.x != 0)
        return (p.x - origin.x) / dir.x;

    if (dir.y != 0)
        return (p.y - origin.y) / dir.y;

    return (p.z - origin.z) / dir.z;
}

#endif //HW_RAY_H
