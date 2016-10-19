#include <fstream>
#include <sstream>
#include "parser.h"

using namespace std;

int Parser::parse_file(
        const string file_name,
        vector<Surface*>& surfaces,
        Camera& camera,
        vector<PointLight*>& lights,
        color& ambient_light
)
{
    ifstream in(file_name);
    if (!in) {
        cerr << "error: no file found" <<  endl;
        return -1;
    }

    /* material is a shared_ptr since many surfaces
     * can share the same material */
    std::shared_ptr<Material> m(nullptr);

    int material_count = 0;
    int camera_count = 0;
    int light_count = 0;
    int ambient_count = 0;

    for (string line; getline(in, line);) {
        if (line.empty() || is_blank(line))
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
                cam.copy_to_camera(camera);
                ++camera_count;
                break;
            }
            case 'm':
            {
                float dr, dg, db, sr, sg, sb, r, ir, ig, ib;
                iss >> dr >> dg >> db >> sr >> sg >> sb >> r >> ir >> ig >> ib;
                m = make_shared<Material>(dr, dg, db, sr, sg, sb, ir, ig, ib, r);
                ++material_count;
                break;
            }
            case 'p':
            {
                float nx, ny, nz, d;
                iss >> nx >> ny >> nz >> d;
                Plane* p = new Plane(nx, ny, nz, d);
                p->material = m;
                surfaces.push_back(p);
                break;
            }
            case 't':
            {
                float ax, ay, az, bx, by, bz, cx, cy, cz;
                iss >> ax >> ay >> az >> bx >> by >> bz >> cx >> cy >> cz;
                Triangle* t = new Triangle(ax, ay, az, bx, by, bz, cx, cy, cz);
                t->material = m;
                surfaces.push_back(t);
                break;
            }
            case 'l':
            {
                char light_type;
                iss >> light_type;

                if (light_type == 'p') {
                    float x, y, z, r, g, b;
                    iss >> x >> y >> z >> r >> g >> b;

                    PointLight* l = new PointLight(x, y, z, r, g, b);
                    lights.push_back(l);
                    ++light_count;
                } else if (light_type == 'a') {
                    float x, y, z;
                    iss >> x >> y >> z;
                    ambient_light.red = x;
                    ambient_light.green = y;
                    ambient_light.blue = z;

                    ++ambient_count;
                } else {
                    // ignore direction lights for now
                }
                break;
            }
            default: continue;
        }
    }

    if (camera_count != 1) {
        cerr << "parse error: scene file should contain only one camera" << endl;
        exit(1);
    }

    if (ambient_count > 1) {
        cerr << "parse error: scene file should contain only one ambient light" << endl;
        exit(1);
    }

    printf("Read %lu surface(s), %d material(s) & %d light(s) \n",
           surfaces.size(), material_count, light_count);

    return 0;
}
