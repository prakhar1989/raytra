#ifndef HW7_PARSER_H
#define HW7_PARSER_H

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>

struct my_vec {
    float x, y, z;
};

inline my_vec operator-(const my_vec& v1, const my_vec& v2)
{
    return { .x = v1.x - v2.x, .y = v1.y - v2.y, .z = v1.z - v2.z };
}

inline my_vec operator+(const my_vec& v1, const my_vec& v2)
{
    return { .x = v1.x + v2.x, .y = v1.y + v2.y, .z = v1.z + v2.z };
}

inline my_vec norm(const my_vec& v)
{
    float s = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    return { .x = v.x / s, .y = v.y / s, .z = v.z / s};
}


inline my_vec cross(const my_vec& v1, const my_vec& v2)
{
    return {
            .x = v1.y * v2.z - v1.z * v2.y,
            .y = v1.z * v2.x - v1.x * v2.z,
            .z = v1.x * v2.y - v1.y * v2.x
    };
};

class Parser {

public:
    static unsigned long parse_obj (
       const std::string& file_name,
       std::vector<float>& vertices,
       std::vector<my_vec>& normals
    );
};


#endif
