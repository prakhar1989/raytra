#include <fstream>
#include <sstream>
#include <assert.h>
#include "parser.h"

using namespace std;

int Parser::parse_file(const string file_name, vector<Surface*>& surfaces, Camera& camera)
{
    ifstream in(file_name);
    if (!in) {
        cerr << "no file\n";
        return -1;
    }

    Material* m = nullptr;
    int material_count = 0;
    int camera_count = 0;

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
                Sphere* s = new Sphere(x, y, z, r);
                if (m == nullptr) {
                    cerr << "invalid scene file. cannot find matching material for surface: " << line << endl;
                    return -1;
                }
                s->material = m;
                surfaces.push_back(s);
                break;
            }
            case 'c':
            {
                float x, y, z, vx, vy, vz, d, iw, ih, pw, ph;
                iss >> x >> y >> z >> vx >> vy >> vz >> d >> iw >> ih >> pw >> ph;
                Camera cam(x, y, z, vx, vy, vz, d, iw, ih, pw, ph);
                camera.nx = cam.nx; camera.ny = cam.ny;
                camera.left = cam.left; camera.right = cam.right;
                camera.top = cam.top; camera.bottom = cam.bottom;
                camera.focal_length = cam.focal_length;

                camera.eye = cam.eye;
                camera.w = cam.w; camera.u = cam.u; camera.v = cam.v;

                ++camera_count;
                break;
            }
            case 'm':
            {
                float dr, dg, db, sr, sg, sb, r, ir, ig, ib;
                iss >> dr >> dg >> db >> sr >> sg >> sb >> r >> ir >> ig >> ib;
                m = new Material(dr, dg, db, sr, sg, sb, ir, ig, ib, r);
                ++material_count;
                break;
            }
            default: continue;
        }
    }

    printf("Read %lu surface(s) and %d material(s)\n", surfaces.size(), material_count);

    if (camera_count != 1)
        std::cerr << "parse error: scene file should contain only one camera" << std::endl;

    return 0;
}
