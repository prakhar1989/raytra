#ifndef HW1_CAMERA_H
#define HW1_CAMERA_H

#include "surface.h"

class Camera {
public:
    Camera();
    Camera(float x, float y, float z,
            float vx, float vy, float vz,
            float d, float iw, float ih, int pw, int ph);

    // for the camera coordinate frame
    Raytra::point eye;
    Raytra::vector u;
    Raytra::vector v;
    Raytra::vector w;

    int nx, ny;

    float focal_length, left, right, top, bottom;
};

#endif //HW1_CAMERA_H
