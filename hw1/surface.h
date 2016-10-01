#ifndef HW1_SURFACE_H
#define HW1_SURFACE_H

struct Point {
    float x, y, z;
};

class Surface {
public:
    virtual void print() = 0;
};

class Sphere: public Surface {
public:
    Sphere(float x, float y, float z, float r);
    Sphere(const Point &center, float radius);
    void print();

    Point center;
    float radius;
};

#endif //HW1_SURFACE_H
