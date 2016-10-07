#include "parser.h"
#include "exr.h"
#include <limits>

using namespace Raytra;
using namespace std;

int get_nearest_surface(const Ray& ray, const vector<Surface*>& surfaces)
{
    float min_t = numeric_limits<float>::infinity();
    int min_index = -1;

    for (int i = 0; i < (int) surfaces.size(); i++) {
        float t = surfaces[i]->get_intersection_point(ray);
        if (t > 0 && t < min_t) {
            min_t = t;
            min_index = i;
        }
    }
    return min_index;
}

int main(int argc, char** argv)
{
    if (argc < 3) {
        cerr << "USAGE: raytra <scene_file> <output_file>" << endl;
        return -1;
    }

    auto version = "0.2";
    printf("Raytra v%s\n", version);

    string scene_file {argv[1]};
    char* output_file {argv[2]};

    vector<Surface*> surfaces;
    Camera camera;

    Parser::parse_file(scene_file, surfaces, camera);

    Array2D<Rgba> pixels;
    pixels.resizeErase(camera.ny, camera.nx);

    for (int y = 0; y < camera.ny; y++) {
        for (int x = 0; x < camera.nx; x++) {

            /* Step 1 - Ray Generation */
            float u = camera.left + (camera.right - camera.left) * (x + 0.5f)/camera.nx;
            float v = camera.bottom + (camera.top - camera.bottom) * (y + 0.5f)/camera.ny;

            Raytra::vec dir = norm((-camera.focal_length * camera.w) + (u * camera.u) +  (v * camera.v));
            Raytra::point origin = camera.eye;
            Ray ray(origin, dir);

            /* Step 2 - Ray Intersection */
            int surface_index = get_nearest_surface(ray, surfaces);


            /* Step 3 - Shading */
            Rgba &px = pixels[y][x];

            if (surface_index != -1) {
                // set the material color as the pixel color
                color c = surfaces[surface_index]->material->diffuse;
                px.r = c.red;
                px.g = c.green;
                px.b = c.blue;
            } else {
                // set black color
                px.r = 0; px.g = 0; px.b = 0;
            }
        }
    }

    printf("Generating image: %s\n", output_file);
    exr::writeRgba(output_file, &pixels[0][0], camera.nx, camera.ny);

}
