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
           verts.size() / 3, tris.size() / 3, file_name.c_str());

    // build the vertices array as an array of floats
    unsigned long n_vertices = tris.size();
    for (auto i = 0; i < n_vertices / 3; i++) {
        for (auto j = 0; j < 3; j++) {
            int vertex = tris[3 * i + j];
            for (auto k = 0; k < 3; k++) {
                vertices.push_back(verts[3 * vertex + k]);
            }
        }
    }

    return n_vertices;
}
