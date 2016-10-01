#ifndef HW1_CAMERA_H
#define HW1_CAMERA_H

#include "surface.h"

class Camera {
public:
    Camera(float x, float y, float z,
            float vx, float vy, float vz,
            float d, float iw, float ih, float pw, float ph);

    Point eye;
    Point direction_point;
    float focal_length;

    dim2d image_plane;
    dim2d pixels_size;
};


#endif //HW1_CAMERA_H
