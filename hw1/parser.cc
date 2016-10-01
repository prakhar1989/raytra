#include <fstream>
#include <sstream>
#include "parser.h"

using namespace std;

int Parser::parse_file(const string file_name, vector<Surface*>& surfaces)
/* parses a file */
{
    ifstream in(file_name);
    if (!in) {
        cerr << "no file\n";
        return -1;
    }

    for (string line; getline(in, line);) {
        if (line.empty())
            continue;

        char cmd;

        istringstream iss(line);
        iss >> cmd;

        switch(cmd) {
            case '/': continue;
            case 's':
            {
                float x, y, z, r;
                iss >> x >> y >> z >> r;
                Sphere sphere(x, y, z, r);
                surfaces.push_back(&sphere);
            }
            case 'c':
                cout << "found camera: " << line << endl;
                break;
            case 'm':
                cout << "found material:" << line << endl;
                break;
            default: continue;
        }
    }
    return 0;
}
