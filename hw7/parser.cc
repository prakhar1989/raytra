#include <sstream>
#include <fstream>
#include "parser.h"

using namespace std;

// returns true if a string is all whitespaces
inline bool is_blank(const std::string& line)
{
    return line.find_first_not_of("\n\t ") == std::string::npos;
}

unsigned long Parser::parse_obj (
        const std::string& file_name,
        std::vector<float> &vertices
)
{
    ifstream in(file_name);

    std::vector<int> tris;
    std::vector<float> verts;

    for (string line; getline(in, line);) {
        if (line.empty() || is_blank(line))
            continue;

        char cmd;

        istringstream iss(line);
        iss >> cmd;

        switch(cmd) {
            case '#': continue;
            case 'v':
            {
                float va, vb, vc;
                iss >> va >> vb >> vc;

                verts.push_back(va);
                verts.push_back(vb);
                verts.push_back(vc);
                break;
            }
            case 'f':
            {
                int i, j, k;
                iss >> i >> j >> k;

                // since indices start at 1
                tris.push_back(i - 1);
                tris.push_back(j - 1);
                tris.push_back(k - 1);
            }
            default: continue;
        }
    }

    in.close();

    printf("Read %lu vertices and %lu faces from '%s'\n",
           vertices.size() / 3, tris.size() / 3, file_name.c_str());

    const unsigned long n_vertices = tris.size();

    for (auto i = 0; i < n_vertices / 3; i++) {
        int v1, v2, v3;
        float x1, y1, z1, x2, y2, z2, x3, y3, z3;

        v1 = tris[3*i];
        v2 = tris[3*i+1];
        v3 = tris[3*i+2];

        x1 = verts[3 * v1];
        y1 = verts[3 * v1 + 1];
        z1 = verts[3 * v1 + 2];

        x2 = verts[3 * v2];
        y2 = verts[3 * v2 + 1];
        z2 = verts[3 * v2 + 2];

        x3 = verts[3 * v3];
        y3 = verts[3 * v3 + 1];
        z3 = verts[3 * v3 + 2];

        vertices.push_back(x1);
        vertices.push_back(y1);
        vertices.push_back(z1);
        vertices.push_back(x2);
        vertices.push_back(y2);
        vertices.push_back(z2);
        vertices.push_back(x3);
        vertices.push_back(y3);
        vertices.push_back(z3);
    }

    return n_vertices;
}
