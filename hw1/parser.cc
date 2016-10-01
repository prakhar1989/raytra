#include <fstream>
#include <sstream>
#include "parser.h"
#include "camera.h"

using namespace std;

int Parser::parse_file(const string file_name, vector<Surface*>& surfaces, Camera* camera)
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
                break;
            }
            case 'c':
            {
                float x, y, z, vx, vy, vz, d, iw, ih, pw, ph;
                iss >> x >> y >> z >> vx >> vy >> vz >> d >> iw >> ih >> pw >> ph;
                Camera cam(x, y, z, vx, vy, vz, d, iw, ih, pw, ph);
                camera = &cam;
                break;
            }
            case 'm':
            {
                float dr, dg, db, sr, sg, sb, r, ir, ig, ib;
                iss >> dr >> dg >> db >> sr >> sg >> sb >> r >> ir >> ig >> ib;
                Material material(dr, dg, db, sr, sg, sb, ir, ig, ib, r);

                // TODO: figure out better ways to assign
                Surface *s = surfaces.at(surfaces.size() - 1);
                s->material = &material;
                break;
            }
            default: continue;
        }
    }
    return 0;
}
