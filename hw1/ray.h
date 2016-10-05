#ifndef HW1_RAY_H
#define HW1_RAY_H

#include "surface.h"

class Ray {
public:
    Ray(const Raytra::point &origin, const Raytra::vector &direction) : origin(origin), dir(direction) {}

    Raytra::point origin;
    Raytra::vector dir;
};

#endif //HW1_RAY_H
