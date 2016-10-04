#include <math.h>

namespace Raytra {
    struct color {
        float red, green, blue;
    };

    struct Point {
        float x, y, z;
    };

    struct vector {
        float x, y, z;
    };

    /* computes the cross product between two vectors */
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

    /* computes the normalized version of a vector */
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
        return { .x = v1.x + v2.x, .y = v1.y + v2.y, .z = v1.z + v2.z, };
    }
}
