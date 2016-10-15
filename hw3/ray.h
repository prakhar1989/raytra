#ifndef HW_RAY_H
#define HW_RAY_H

using namespace Raytra;

class Ray {
public:
    Ray(const point &origin, const vec &direction) : origin(origin), dir(direction) {}
    point get_point(float t) const;

    point origin;
    vec dir;
};

inline point Ray::get_point(float t) const {
    return t * dir + origin;
}

#endif //HW_RAY_H
