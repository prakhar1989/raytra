#ifndef HW1_RAY_H
#define HW1_RAY_H

#include "surface.h"

class Ray {
public:
    Ray(const Raytra::Point &origin, const Raytra::vector &direction) : origin(origin), direction(direction) {}

    Raytra::Point origin;
    Raytra::vector direction;
};

#endif //HW1_RAY_H
