#include "parser.h"

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
    for (int i = 0; i < surfaces.size(); i++) {
        float d = surfaces[i]->get_intersection_point(ray);
        if (d != -1) {
            printf("Ray hit surface: %d at d: %f", i, d);
        }
    }
    return 0;
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "USAGE: raytra <scene_file> <output_file>" << std::endl;
        return -1;
    }

    auto version = "0.1";

    std::string scene_file {argv[1]};
    std::vector<Surface*> surfaces;
    Camera camera;

    Parser::parse_file(scene_file, surfaces, camera);

    for (auto s: surfaces) {
        s->print();
        std::cout << s->material->diffuse << std::endl;
        std::cout << s->material->specular << std::endl;
        std::cout << s->material->ideal_specular << std::endl;
    }

    for (int i = 0; i < camera.nx; i++) {
        for (int j = 0; j < camera.ny; j++) {

            float u = camera.left + (camera.right - camera.left) * (i + 0.5f)/camera.nx;
            float v = camera.bottom + (camera.top - camera.bottom) * (j + 0.5f)/camera.ny;

            Raytra::vector dir = (-camera.focal_length * camera.w) +
                                 (u * camera.u) +  (v * camera.v);

            Raytra::point origin = camera.eye;

            std::cout << origin << std::endl;
            std::cout << dir << std::endl;

            Ray r(origin, dir);
        }
    }
}
