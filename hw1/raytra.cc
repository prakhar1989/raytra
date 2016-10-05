#include "parser.h"
#include "exr.h"

using namespace Raytra;

std::ostream& operator << (std::ostream& o, const point& p)
{
    return o << "Point(x: " << p.x << ",y: " << p.y << ", z: " << p.z << ")";
}

std::ostream& operator << (std::ostream& o, const vector& v)
{
    return o << "Vector(x: " << v.x << ",y: " << v.y << ", z: " << v.z << ")";
}

std::ostream& operator << (std::ostream& o, const color& c)
{
    return o << "Color(r: " << c.red
             << ",y: " << c.green << ", z: " << c.blue << ")";
}

int get_nearest_surface(const Ray& ray, const std::vector<Surface*>& surfaces)
{
    float min_t = std::numeric_limits<float>::infinity();
    int min_index = -1;

    for (int i = 0; i < surfaces.size(); i++) {
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
        std::cerr << "USAGE: raytra <scene_file> <output_file>" << std::endl;
        return -1;
    }

    auto version = "0.1";
    printf("Raytra v%s\n", version);

    std::string scene_file {argv[1]};
    char* output_file {argv[2]};

    std::vector<Surface*> surfaces;
    Camera camera;

    Parser::parse_file(scene_file, surfaces, camera);

    Array2D<Rgba> pixels;
    pixels.resizeErase(camera.ny, camera.nx);

    for (int y = 0; y < camera.ny; y++) {
        for (int x = 0; x < camera.nx; x++) {

            /* Step 1 - Ray Generation */
            float u = camera.left + (camera.right - camera.left) * (x + 0.5f)/camera.nx;
            float v = camera.bottom + (camera.top - camera.bottom) * (y + 0.5f)/camera.ny;

            Raytra::vector dir = norm((-camera.focal_length * camera.w) + (u * camera.u) +  (v * camera.v));
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
