#ifndef HW_CAMERA_H
#define HW_CAMERA_H

#include "surface.h"

using namespace Raytra;

class Camera {
public:
    Camera();
    Camera(float x, float y, float z,
            float vx, float vy, float vz,
            float d, float iw, float ih, int pw, int ph);

    point eye; vec u; vec v; vec w;

    int nx, ny;

    float focal_length, left, right, top, bottom;
};

#endif //HW_CAMERA_H
