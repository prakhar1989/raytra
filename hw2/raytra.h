#ifndef HW_RAYTRA_H
#define HW_RAYTRA_H

#include <math.h>
#include <iostream>

namespace Raytra {
    struct color {
        float red, green, blue;
    };

    struct point {
        float x, y, z;
    };

    struct vec {
        float x, y, z;
    };

    /* --- VECTOR OPERATIONS ----- */
    inline vec cross(const vec& v1, const vec& v2)
    {
        return {
            .x = v1.y * v2.z - v1.z * v2.y,
            .y = v1.x * v2.z - v1.z * v2.x,
            .z = v1.x * v2.y - v1.y * v2.x
        };
    };

    inline float dot(const vec& v1, const vec& v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    inline vec operator-(const vec& v)
    {
        return { .x = -v.x, .y = -v.y, .z = -v.z };
    }

    inline vec norm(const vec& v)
    {
        float s = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
        return { .x = v.x / s, .y = v.y / s, .z = v.z / s};
    }

    inline vec operator*(float c, const vec& v)
    {
        return { .x = v.x * c, .y = v.y * c, .z = v.z * c };
    }

    inline vec operator+(const vec& v1, const vec& v2)
    {
        return { .x = v1.x + v2.x, .y = v1.y + v2.y, .z = v1.z + v2.z };
    }

    inline point operator+(const vec& v1, const point& p1)
    {
        return { .x = v1.x + p1.x, .y = v1.y + p1.y, .z = v1.z + p1.z };
    }

    /* --- POINT OPERATIONS ----- */
    inline vec operator-(const point& p1, const point& p2)
    {
        return { .x = p1.x - p2.x, .y = p1.y - p2.y, .z = p1.z - p2.z };
    }

    inline vec operator+(const point& p1, const point& p2)
    {
        return { .x = p1.x + p2.x, .y = p1.y + p2.y, .z = p1.z + p2.z };
    }

    inline std::ostream& operator << (std::ostream& o, const point& p)
    {
        return o << "Point(x: " << p.x << ",y: " << p.y << ", z: " << p.z << ")";
    }

    inline std::ostream& operator << (std::ostream& o, const vec& v)
    {
        return o << "Vector(x: " << v.x << ",y: " << v.y << ", z: " << v.z << ")";
    }

    inline std::ostream& operator << (std::ostream& o, const color& c)
    {
        return o << "Color(r: " << c.red
                 << ",y: " << c.green << ", z: " << c.blue << ")";
    }
}

#endif
