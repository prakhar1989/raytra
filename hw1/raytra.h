#include <math.h>

namespace Raytra {
    struct color {
        float red, green, blue;
    };

    struct point {
        float x, y, z;
    };

    struct vector {
        float x, y, z;
    };

    /* --- VECTOR OPERATIONS ----- */
    inline vector cross(const vector& v1, const vector& v2)
    {
        return {
            .x = v1.y * v2.z - v1.z * v2.y,
            .y = v1.x * v2.z - v1.z * v2.x,
            .z = v1.x * v2.y - v1.y * v2.x
        };
    };

    inline float dot(const vector& v1, const vector& v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    inline vector operator-(const vector& vec)
    {
        return { .x = -vec.x, .y = -vec.y, .z = -vec.z };
    }

    inline vector norm(const vector& vec)
    {
        float s = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
        return { .x = vec.x / s, .y = vec.y / s, .z = vec.z / s};
    }

    inline vector operator*(float c, const vector& vec)
    {
        return { .x = vec.x * c, .y = vec.y * c, .z = vec.z * c };
    }

    inline vector operator+(const vector& v1, const vector& v2)
    {
        return { .x = v1.x + v2.x, .y = v1.y + v2.y, .z = v1.z + v2.z };
    }

    inline point operator+(const vector& v1, const point& p1)
    {
        return { .x = v1.x + p1.x, .y = v1.y + p1.y, .z = v1.z + p1.z };
    }

    /* --- POINT OPERATIONS ----- */
    inline vector operator-(const point& p1, const point& p2)
    {
        return { .x = p1.x - p2.x, .y = p1.y - p2.y, .z = p1.z - p2.z };
    }

    inline vector operator+(const point& p1, const point& p2)
    {
        return { .x = p1.x + p2.x, .y = p1.y + p2.y, .z = p1.z + p2.z };
    }
}
