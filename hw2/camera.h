#ifndef HW_CAMERA_H
#define HW_CAMERA_H

#include "surface.h"

using namespace Raytra;

class Camera {
public:
    Camera();
    virtual ~Camera() {};

    Camera(float x, float y, float z,
            float vx, float vy, float vz,
            float d, float iw, float ih, int pw, int ph);

    void copy_to_camera(Camera& cam);
    vec ray_direction(float x, float y);
    int pixelsX();
    int pixelsY();
    point get_center();
private:
    vec u; vec v; vec w;
    point eye;
    float focal_length, left, right, top, bottom;
    int nx, ny;
};

#endif //HW_CAMERA_H
