#include "parser.h"
#include "exr.h"
#include "point_light.h"
#include <limits>

using namespace Raytra;
using namespace std;

void cleanup(vector<Surface*>& surfaces)
{
    for (auto s: surfaces)
        delete s;
}

/* returns a pair of index and t for the intersection point
 * at the nearest surface
 */
pair<int, float> get_nearest_surface (
        const Ray& ray,
        const vector<Surface*>& surfaces
)
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
    return make_pair(min_index, min_t);
}

color compute_shading (
       const Surface* surface,
       const Ray& ray,
       const Raytra::point& point,
       const Point_light& light,
       const color& ambient_light
)
{
    // diffuse computation
    vec surface_normal = surface->get_normal(point);
    vec light_ray = norm(light.position - point);
    float cosine = fmaxf(0, dot(surface_normal, light_ray));
    color kd = surface->material->diffuse;

    float d2 = powf(dist(light.position, point), 2);
    color diffuse = {
            .red   = kd.red * light.c.red * cosine * light.intensity,
            .green = kd.green * light.c.green * cosine * light.intensity,
            .blue  = kd.blue * light.c.blue * cosine * light.intensity,
    };

    // specular computation
    vec bisector = norm(-ray.dir + light_ray);
    float cosalpha = fmaxf(0, dot(surface_normal, bisector));
    float phong = surface->material->phong;
    float multiplier = light.intensity * powf(cosalpha, phong);
    color ks = surface->material->specular;
    color specular = {
            .red = ks.red * multiplier * light.c.red,
            .green = ks.green * multiplier * light.c.green,
            .blue = ks.blue * multiplier * light.c.blue,
    };

    return {
            .red = (diffuse.red + specular.red)/d2,
            .green = (diffuse.green + specular.green)/d2,
            .blue = (diffuse.blue + specular.blue)/d2,
    };
}

int main(int argc, char** argv)
{
    if (argc < 3) {
        cerr << "USAGE: raytra <scene_file> <output_file>" << endl;
        return -1;
    }

    auto version = "0.3";
    printf("Raytra v%s\n", version);

    string scene_file {argv[1]};
    char* output_file {argv[2]};

    vector<Surface*> surfaces;
    Camera camera;
    Point_light point_light;
    color ambient_light;

    Parser::parse_file(scene_file, surfaces, camera, point_light, ambient_light);

    Array2D<Rgba> pixels;
    pixels.resizeErase(camera.pixelsY(), camera.pixelsX());

    for (int y = 0; y < camera.pixelsY(); y++) {
        for (int x = 0; x < camera.pixelsX(); x++) {
            /* Step 1 - Ray Generation */
            vec dir = camera.ray_direction(x, y);
            point origin = camera.get_center();
            Ray ray(origin, dir);

            /* Step 2 - Ray Intersection */
            pair<int, float> hit = get_nearest_surface(ray, surfaces);
            int surface_index = hit.first;

            /* Step 3 - Shading */
            Rgba &px = pixels[y][x];

            if (surface_index != -1) {
                Surface* surface = surfaces[surface_index];
                point intersection_pt = ray.get_point(hit.second);

                color c = compute_shading(surface,
                                          ray,
                                          intersection_pt,
                                          point_light,
                                          ambient_light);
                px.r = c.red;
                px.g = c.green;
                px.b = c.blue;
                px.a = 1;
            } else {
                // set black color
                px.r = 0; px.g = 0; px.b = 0; px.a = 1;
            }
        }
    }

    printf("Generating image: %s\n", output_file);
    exr::writeRgba(output_file, &pixels[0][0],
                   camera.pixelsX(), camera.pixelsY());

    // memory cleanup
    cleanup(surfaces);
}
