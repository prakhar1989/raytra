#ifndef HW_RAY_H
#define HW_RAY_H

class Ray {
public:
    Ray(const Raytra::point &origin, const Raytra::vec &direction) : origin(origin), dir(direction) {}

    Raytra::point get_point(float t) const;

    Raytra::point origin;
    Raytra::vec dir;
};

inline Raytra::point Ray::get_point(float t) const {
    return t * dir + origin;
}

#endif //HW_RAY_H
