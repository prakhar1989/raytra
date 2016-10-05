#ifndef HW1_RAY_H
#define HW1_RAY_H

class Ray {
public:
    Ray(const Raytra::point &origin, const Raytra::vector &direction) : origin(origin), dir(direction) {}

    Raytra::point get_point(float t) const;

    Raytra::point origin;
    Raytra::vector dir;
};

inline Raytra::point Ray::get_point(float t) const {
    return t * dir + origin;
}

#endif //HW1_RAY_H
