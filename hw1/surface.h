#ifndef HW1_SURFACE_H
#define HW1_SURFACE_H

struct Point {
    float x, y, z;
};

class Surface {
    virtual void print() = 0;
};

#endif //HW1_SURFACE_H
