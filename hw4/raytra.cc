#include "parser.h"
#include "exr.h"
#include <fstream>
#include "point_light.h"
#include <limits>
#include "ProgressBar.hpp"

using namespace Raytra;
using namespace std;

const int MAX_REFLECTIONS = 20;

void cleanup(vector<Surface*>& surfaces,
             vector<PointLight*>& lights)
{
    for (auto s: surfaces)
        delete s;

    for (auto l: lights)
        delete l;
}

/**
 * Checks if a file exist
 */
bool does_file_exist(const string& filename)
{
    std::ifstream infile(filename);
    return infile.good();
}


/**
 * @param ray - the camera ray for which the nearest surface has to be computed
 * @param surfaces - a vector of surfaces that needs to be searched over
 * @param incident_surface_index - index of the surface that shouldn't
 * be counted in the computation.
 *
 * @returns - a pair of index and parameter t for the intersection point
 * at the nearest surface
 */
pair<int, float> get_nearest_surface (
        const Ray& ray,
        const vector<Surface*>& surfaces,
        int incident_surface_index
)
{
    float min_t = numeric_limits<float>::infinity();
    int min_index = -1;

    for (int i = 0; i < (int) surfaces.size(); i++) {
        float t = surfaces[i]->get_intersection_point(ray);
        if (t > 0.001 && t < min_t &&
                i != incident_surface_index) {
            min_t = t;
            min_index = i;
        }
    }
    return make_pair(min_index, min_t);
}


/**
 * Computes the spectral power distribution using the phong
 * shading model and reflections
 * @param ray - camera ray
 * @param surfaces - list of all surfaces in the scene
 * @param lights - list of all lights in the scene
 * @param bounces_left - number of light bounces left
 * @param incident_surface_index - index of the incident surface
 * useful for making sure surfaces don't reflect themselves.
 * @return
 */
color compute_spd (
    const Ray &ray,
    const vector<Surface *> &surfaces,
    const vector<PointLight *> &lights,
    int bounces_left,
    int incident_surface_index
)
{
    color spd = {.red = 0, .green = 0, .blue = 0};

    if (bounces_left == 0)
        return spd;

    /* Step 2 - Ray Intersection */
    pair<int, float> hit = get_nearest_surface(ray, surfaces, incident_surface_index);
    int surface_index = hit.first;

    /* no intersection - color black */
    if (surface_index < 0)
        return spd;

    /* on intersection, compute shading
     * by summing contributions from each light source
     */
    Surface* surface = surfaces[surface_index];
    point intersection_pt = ray.get_point(hit.second);
    color c;

    for (auto light: lights) {
        /* compute shading only if the light to the surface
         * at the intersection point is not occluded by another surface
         */
        if (!light->is_occluded_by(intersection_pt, surfaces)) {
            c = light->compute_shading(surface, ray, intersection_pt);
            spd.red += c.red;
            spd.green += c.green;
            spd.blue += c.blue;
        }
    }

    color reflective = surface->material->ideal_specular;

    /* not reflective surface; return */
    if (!surface->material->is_reflective())
        return spd;

    /* compute the reflected ray */
    vec N = surface->get_normal(intersection_pt);
    vec reflect_dir = norm(ray.dir + (-2 * dot(ray.dir, N) * N));
    Ray reflected_ray(intersection_pt, reflect_dir);

    /* recursively compute reflection shading */
    color reflected_spd = compute_spd(reflected_ray, surfaces,
                                     lights, bounces_left - 1,
                                     surface_index);

    return {
        .red = spd.red + reflected_spd.red * reflective.red,
        .green = spd.green + reflected_spd.green * reflective.green,
        .blue = spd.blue + reflected_spd.blue * reflective.blue
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

    if (!does_file_exist(scene_file)) {
        cerr << "error: scene file doesn't exist" << endl;
        return -1;
    }

    vector<Surface*> surfaces;
    vector<PointLight*> lights;
    Camera camera;
    color ambient_light;

    Parser::parse_file(scene_file, surfaces, camera, lights, ambient_light);

    Array2D<Rgba> pixels;
    pixels.resizeErase(camera.pixelsY(), camera.pixelsX());

    ProgressBar progressBar(camera.pixelsX() * camera.pixelsY(), 70);

    cout << "Rendering..." << endl;

    for (int y = 0; y < camera.pixelsY(); y++) {
        for (int x = 0; x < camera.pixelsX(); x++) {

            /* Step 0: show progress */
            if (++progressBar % 1000 == 0)
                progressBar.display();

            /* step 1: generate ray */
            vec dir = camera.ray_direction(x, y);
            point origin = camera.get_center();
            Ray ray(origin, dir);

            /* compute compute_spd */
            color c = compute_spd(ray, surfaces, lights, MAX_REFLECTIONS, -1);

            /* finally assign shading to the pixel */
            Rgba &px = pixels[y][x];
            px.r = c.red; px.g = c.green; px.b = c.blue; px.a = 1;

            /* add ambient light */
            px.r += ambient_light.red;
            px.g += ambient_light.green;
            px.b += ambient_light.blue;
        }
    }

    exr::writeRgba(output_file, &pixels[0][0],
                   camera.pixelsX(), camera.pixelsY());

    progressBar.display();
    printf("\nImage generated: %s\n", output_file);

    /* cleanup up surfaces */
    cleanup(surfaces, lights);
}
