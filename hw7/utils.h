#ifndef HW7_UTILS_H
#define HW7_UTILS_H
#include <fstream>

typedef vec4 point4;
typedef vec4 color4;

struct light_properties {
    point4 position;
    color4 ambient;
    color4 diffuse;
    color4 specular;
};

struct material_properties {
    color4 ambient;
    color4 diffuse;
    color4 specular;
    float shininess;
};

void inline error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

// three helper functions for the vec4 class:
void inline vecproduct (vec4 &res, const vec4 &v1, const vec4 &v2) {
    for (int i = 0; i < 4; ++i)
        res[i] = v1[i] * v2[i];
}

void inline vecset(vec4 &res, const vec4 &v1)
{
    for (int i = 0; i < 4; ++i)
        res[i] = v1[i];
}

void inline vecclear(vec4 &res)
{
    for (int i = 0; i < 3; ++i)
        res[i] = 0.0;
    res[3] = 1.0;
}


/**
 * Checks if a file exist
 */
bool does_file_exist(const std::string& filename)
{
    std::ifstream infile(filename);
    return infile.good();
}

#endif
