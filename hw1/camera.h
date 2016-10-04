#ifndef HW1_CAMERA_H
#define HW1_CAMERA_H

#include "surface.h"

class Camera {
public:
    Camera(float x, float y, float z,
            float vx, float vy, float vz,
            float d, float iw, float ih, float pw, float ph);

    // for the camera coordinate frame
    Raytra::Point eye;
    Raytra::vector u;
    Raytra::vector v;
    Raytra::vector w;

    float focal_length;

    Raytra::dim2d image_plane;
    Raytra::dim2d pixels_size;
};

#endif //HW1_CAMERA_H
