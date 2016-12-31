#include "parser.h"
#include "exr.h"
#include <fstream>
#include "point_light.h"
#include "bounding_box.h"
#include "area_light.h"
#include <limits>
#include "ProgressBar.hpp"
#include "BVHTree.h"

using namespace Raytra;
using namespace std;

const int MAX_REFLECTIONS = 20;
const unsigned int ray_samples = 4;
const unsigned int shadow_samples = 4;

void cleanup(vector<Surface*>& surfaces,
             vector<PointLight*>& lights,
             vector<AreaLight*>& area_lights,
             BVHTree* tree)
{
    for (auto s: surfaces)
        delete s;

    for (auto l: lights)
        delete l;

    for (auto l: area_lights)
        delete l;

    delete tree;
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
        BVHTree* tree
)
{
    float min_t = numeric_limits<float>::infinity();
    int min_index = -1;

    vector<unsigned int> surface_indices;
    if (tree != nullptr) {
        tree->compute_intersections(ray, surface_indices);
    } else {
        for (unsigned int j = 0; j < surfaces.size(); j++)
            surface_indices.push_back(j);
    }

    for (auto i: surface_indices) {
        float t = surfaces[i]->get_intersection_point(ray);
        if (t > 0.001 && t < min_t ) {
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
 * @param point_lights - list of all point lights in the scene
 * @param area_lights - list of all area lights in the scene
 * @param bounces_left - number of light bounces left
 * @param incident_surface_index - index of the incident surface
 * useful for making sure surfaces don't reflect themselves.
 * @param tree - the bounding volume hierarchy tree for the scene
 * @return
 */
color compute_spd (
    const Ray &ray,
    const vector<Surface *> &surfaces,
    const vector<PointLight *> &point_lights,
    const vector<AreaLight *> &area_lights,
    const color& ambient_light,
    unsigned int shadow_ray_samples,
    int bounces_left,
    int incident_surface_index,
    BVHTree* tree
)
{
    color spd = {.red = 0, .green = 0, .blue = 0};

    if (bounces_left == 0)
        return spd;

    /* Step 2 - Ray Intersection */
    pair<int, float> hit = get_nearest_surface(ray, surfaces, tree);
    int surface_index = hit.first;

    /* no intersection - color black */
    if (surface_index < 0)
        return spd;

    /* Step 3 - Compute various contributions at intersection point */
    Surface* surface = surfaces[surface_index];
    point intersection_pt = ray.get_point(hit.second);
    color c;

    /** POINT LIGHTS COMPUTATION */
    /* computing spectral power distribution contribution by
     * point lights which generate hard shadows */
    for (auto light: point_lights) {
        if (!light->is_occluded_by(intersection_pt, surfaces, tree)) {
            c = light->compute_shading(surface, ray, intersection_pt);
            spd.red += c.red;
            spd.green += c.green;
            spd.blue += c.blue;
        }
    }

    /** AREAL LIGHTS COMPUTATION */
    /* computing specular power distribution contribution by
     * area lights which generate soft shadows */
    for (auto light: area_lights) {
        for (unsigned int i = 0; i < shadow_ray_samples; i++) {
            for (unsigned int j = 0; j < shadow_ray_samples; j++) {

                /* compute point located on the light */
                point point_on_light = light->get_point(i, j, shadow_ray_samples);

                if (!light->is_occluded_by(point_on_light, intersection_pt,surfaces, tree)) {
                    c = light->compute_shading(
                            surface, ray,
                            intersection_pt, point_on_light, shadow_ray_samples
                    );
                    spd.red += c.red; spd.green += c.green; spd.blue += c.blue;
                }
            }
        }
    }

    /** AMBIENT LIGHTING COMPUTATION */
    if (incident_surface_index == -1) {
        // add ambient only once. this condition ensures
        // that the ambient light is added only to the primary
        // camera ray
        spd.red += surface->material->diffuse.red * ambient_light.red;
        spd.green += surface->material->diffuse.green * ambient_light.green;
        spd.blue += surface->material->diffuse.blue * ambient_light.blue;
    }

    /** REFLECTIVE LIGHTING COMPUTATION */
    color reflective = surface->material->ideal_specular;

    /* not reflective surface; return */
    if (!surface->material->is_reflective() || !surface->is_front_facing(ray))
        return spd;

    /* compute the reflected ray */
    vec N = surface->get_normal(intersection_pt);

    vec reflect_dir = norm(ray.dir + (-2 * dot(ray.dir, N) * N));
    Ray reflected_ray(intersection_pt, reflect_dir);

    /* recursively compute reflection shading */
    color reflected_spd = compute_spd (
            reflected_ray, surfaces,
            point_lights, area_lights, ambient_light,
            shadow_ray_samples, bounces_left - 1,
            surface_index, tree
    );

    /** TOTAL CONTRIBUTION */
    return {
        .red = spd.red + reflected_spd.red * reflective.red,
        .green = spd.green + reflected_spd.green * reflective.green,
        .blue = spd.blue + reflected_spd.blue * reflective.blue
    };
}

int main(int argc, char** argv)
{
    if (argc < 3) {
        cerr << "USAGE: raytra <scene_file> <output_file> " << endl;
        return -1;
    }

    auto version = "0.5";
    printf("Raytra v%s\n", version);

    string scene_file {argv[1]};
    char* output_file {argv[2]};

    if (!does_file_exist(scene_file)) {
        cerr << "error: scene file doesn't exist" << endl;
        return -1;
    }

    /* initialize RNG */
    srand(1);

    /* setting up objects in the scene */
    vector<Surface*> surfaces;
    vector<PointLight*> point_lights;
    vector<AreaLight*> area_lights;
    Camera camera;
    color ambient_light;

    /* parse the scene file & populate the objects */
    Parser::parse_file(scene_file, surfaces, camera,
                       point_lights, area_lights, ambient_light);

    /* set up a collection of bounding boxes */
    vector<BoundingBox*> bounding_boxes;
    for (unsigned int i = 0; i < surfaces.size(); i++) {
        BoundingBox* box = surfaces[i]->get_bounding_box();
        box->set_surface_index(i);
        bounding_boxes.push_back(box);
    }

    /* construct the BVHTree using the bounding boxes */
    BVHTree* tree = BVHTree::make_bvhtree(bounding_boxes.begin(), bounding_boxes.end(), Axis::X);

    Array2D<Rgba> pixels;
    pixels.resizeErase(camera.pixelsY(), camera.pixelsX());

    ProgressBar progressBar(camera.pixelsX() * camera.pixelsY(), 70);

    cout << "Rendering..." << endl;

    /* The number of stratas we devide each pixel into.
     * for implementing stratefied monte-carlo sampling */
    auto strata_size = ray_samples * ray_samples;

    for (int y = 0; y < camera.pixelsY(); y++) {
        for (int x = 0; x < camera.pixelsX(); x++) {

            /* Step 0: update progress bar */
            if (++progressBar % 1000 == 0)
                progressBar.display();

            color c = {0, 0, 0};

            for (unsigned int i = 0; i < ray_samples; i++) {
                for (unsigned int j = 0; j < ray_samples; j++) {

                    /* step 1: generate ray */
                    vec dir = camera.ray_direction(x, y, i, j, ray_samples);
                    point origin = camera.get_center();
                    Ray ray(origin, dir);

                    /* compute spectral power distribution */
                    c = c + compute_spd (
                            ray, surfaces, point_lights, area_lights,
                            ambient_light, shadow_samples,
                            MAX_REFLECTIONS, -1, tree
                    );
                }
            }

            Rgba &px = pixels[y][x];
            px.a = 1;
            px.r = c.red/strata_size;
            px.g = c.green/strata_size;
            px.b = c.blue/strata_size;
        }
    }

    exr::writeRgba(output_file, &pixels[0][0], camera.pixelsX(), camera.pixelsY());

    progressBar.display();
    printf("\nImage generated: %s\n", output_file);

    /* cleanup up surfaces */
    cleanup(surfaces, point_lights, area_lights, tree);
}
