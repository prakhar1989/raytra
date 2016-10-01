#include "camera.h"

Camera::Camera(float x, float y, float z,
               float vx, float vy, float vz,
               float d, float iw, float ih, float pw, float ph)
{
    eye = {.x = x, .y = y, .z = z};
    direction_point = {.x = vx, .y = vy, .z = vz};
    focal_length = d;
    image_plane = {.width = iw, .height = ih};
    pixels_size = {.width = pw, .height = ph};
}
