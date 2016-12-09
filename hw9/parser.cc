#include "parser.h"

using namespace std;

// returns true if a string is all whitespaces
inline bool is_blank(const std::string& line)
{
    return line.find_first_not_of("\n\t ") == std::string::npos;
}

unsigned long Parser::parse_obj (
        const std::string& file_name,
        std::vector<float> &vertices,
        std::vector<my_vec> &norms
)
{
    ifstream in(file_name);

    std::vector<int> tris;
    std::vector<float> verts;
    std::vector<my_vec> normals;

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
    unsigned long n_faces = tris.size();
    unsigned long n_counts = verts.size()/3;

    for (auto i = 0; i < n_counts; i++)
        normals.push_back({0, 0, 0});

    for (auto i = 0; i < n_faces / 3; i++) {
        int a_index = tris[3*i];
        int b_index = tris[3*i+1];
        int c_index = tris[3*i+2];

        float a1 = verts[3*a_index];
        float a2 = verts[3*a_index+1];
        float a3 = verts[3*a_index+2];
        my_vec a = {a1, a2, a3};

        float b1 = verts[3*b_index];
        float b2 = verts[3*b_index+1];
        float b3 = verts[3*b_index+2];
        my_vec b = {b1, b2, b3};

        float c1 = verts[3*c_index];
        float c2 = verts[3*c_index+1];
        float c3 = verts[3*c_index+2];
        my_vec c = {c1, c2, c3};

        // computing the normal
        my_vec e1 = b - a;
        my_vec e2 = c - a;
        my_vec n =  norm(cross(e1, e2));

        normals[a_index] = normals[a_index] + n;
        normals[b_index] = normals[b_index] + n;
        normals[c_index] = normals[c_index] + n;

        vertices.push_back(a1);
        vertices.push_back(a2);
        vertices.push_back(a3);
        vertices.push_back(b1);
        vertices.push_back(b2);
        vertices.push_back(b3);
        vertices.push_back(c1);
        vertices.push_back(c2);
        vertices.push_back(c3);
    }

    // normalize
    for (unsigned int i = 0; i < n_counts; i++)
        norm(normals[i]);

    for (unsigned int i = 0; i < n_faces/3; i++) {
        int a_index = tris[3*i];
        int b_index = tris[3*i+1];
        int c_index = tris[3*i+2];

        norms.push_back(normals[a_index]);
        norms.push_back(normals[b_index]);
        norms.push_back(normals[c_index]);
    }

    return n_faces;
}
