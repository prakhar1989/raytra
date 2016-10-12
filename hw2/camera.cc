#include "camera.h"

using namespace Raytra;

Camera::Camera() {}

Camera::Camera(float x, float y, float z,
               float vx, float vy, float vz,
               float d, float iw, float ih, int pw, int ph)
{
    eye = {.x = x, .y = y, .z = z};

    vec direction = {.x = vx, .y = vy, .z = vz};
    vec up = {.x = 0, .y = 1, .z = 0};

    w = norm(-(direction));
    u = norm(cross(direction, up));
    v = norm(cross(direction, u));

    focal_length = d;

    nx = pw; ny = ph;

    left = -iw/2; bottom = -ih/2; right = iw/2; top = ih/2;
}
