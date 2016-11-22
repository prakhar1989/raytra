#include <sstream>
#include <fstream>
#include "parser.h"

using namespace std;

// returns true if a string is all whitespaces
inline bool is_blank(const std::string& line)
{
    return line.find_first_not_of("\n\t ") == std::string::npos;
}

int Parser::parse_obj(
        const std::string& file_name,
        std::vector<int> &tris,
        std::vector<float> &vertices
)
{
    ifstream in(file_name);

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

                vertices.push_back(va);
                vertices.push_back(vb);
                vertices.push_back(vc);
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
    return 0;
}
