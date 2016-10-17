#include "parser.h"
#include "exr.h"
#include "point_light.h"
#include <limits>

using namespace Raytra;
using namespace std;

void cleanup(vector<Surface*>& surfaces,
             vector<PointLight*>& lights)
{
    for (auto s: surfaces)
        delete s;

    for (auto l: lights)
        delete l;
}

/**
 * @param ray - the camera ray for which the nearest surface has to be computed
 * @param surfaces - a vector of surfaces that needs to be searched over
 *
 * @returns - a pair of index and parameter t for the intersection point
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

/**
 * Computes diffuse shading at a surface for a point light
 *
 * @param surface - the surface for which shading has to be computed
 * @param point - the point at which the camera ray and surface intersect
 * @param ray - the camera ray at the point of intersection
 * @param light - a point light at that point due to which shading occurs
 *
 * @Returns - a color (r,g,b triple) to denote the shading at the point
 */
color diffuse_shading (
        const Surface* surface,
        const Ray& ray,
        const Raytra::point& point,
        const PointLight* light
)
{
    // diffuse computation
    vec surface_normal = surface->get_normal(point);
    vec light_ray = norm(light->position - point);
    float cosine = fmaxf(0, dot(surface_normal, light_ray));
    color kd = surface->material->diffuse;

    return {
            .red   = kd.red * light->c.red * cosine * light->intensity,
            .green = kd.green * light->c.green * cosine * light->intensity,
            .blue  = kd.blue * light->c.blue * cosine * light->intensity,
    };
}


/**
 * Computes specular shading at a surface for a point light
 *
 * @param surface - the surface for which shading has to be computed
 * @param point - the point at which the camera ray and surface intersect
 * @param ray - the camera ray at the point of intersection
 * @param light - a point light at that point due to which shading occurs
 *
 * @Returns - a color (r,g,b triple) to denote the shading at the point
 */
color specular_shading (
        const Surface* surface,
        const Ray& ray,
        const Raytra::point& point,
        const PointLight* light
)
{
    vec surface_normal = surface->get_normal(point);
    vec light_ray = norm(light->position - point);
    vec bisector = norm(-ray.dir + light_ray);

    float cosalpha = fmaxf(0, dot(surface_normal, bisector));
    float multiplier = light->intensity * powf(cosalpha, surface->material->phong);
    color ks = surface->material->specular;
    return {
            .red = ks.red * multiplier * light->c.red,
            .green = ks.green * multiplier * light->c.green,
            .blue = ks.blue * multiplier * light->c.blue,
    };
}

/**
 * Checks whether a point on a surface is
 * occuled by any other surface between the point
 * and the light. Used for computing shadows.
 *
 * @param point - point of intersection
 * @param surfaces - a vector of surfaces in the scene
 * @param light - a point light from which the occlusion has
 * to be calculated
 * @return true to indicate if the surface is occluded.
 */
bool is_occluded_by (
        const Raytra::point& point,
        const vector<Surface*>& surfaces,
        const PointLight* light
)
{
    /* to avoid shadow rounding errors */
    const float surface_delta = 0.01;

    /* generate ray from point to light */
    vec light_dir = norm(light->position - point);
    Ray light_ray(point, light_dir);

    /* the t at which the light is located */
    float t_light = light_ray.offset(light->position);

    /* compute intersection of light ray with all surfaces */
    for(auto surface: surfaces) {
        float t = surface->get_intersection_point(light_ray);
        if (t > 0 && t < t_light && fabsf(t) > surface_delta)
            return true;
    }

    return false;
}

/**
 * Computes diffuse and specular shading at a surface for a point light
 *
 * @param surface - the surface for which shading has to be computed
 * @param point - the point at which the camera ray and surface intersect
 * @param ray - the camera ray at the point of intersection
 * @param light - a point light at that point due to which shading occurs
 *
 * @Returns - a color (r,g,b triple) to denote the shading at the point
 */
color compute_shading (
       const Surface* surface,
       const Ray& ray,
       const Raytra::point& point,
       const PointLight* light
)
{
    const float d2 = powf(dist(light->position, point), 2);
    const color diffuse  = diffuse_shading(surface, ray, point, light);
    const color specular = specular_shading(surface, ray, point, light);

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
    vector<PointLight*> lights;
    Camera camera;
    color ambient_light;

    Parser::parse_file(scene_file, surfaces, camera, lights, ambient_light);

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
            px.r = 0; px.g = 0; px.b = 0; px.a = 1;

            /* no intersection - color black */
            if (surface_index < 0)
                continue;

            /* on intersection, compute shading
             * by summing contributions from each light source
             */
            Surface* surface = surfaces[surface_index];
            point intersection_pt = ray.get_point(hit.second);
            color c;

            for (auto light: lights) {
                if (!is_occluded_by(intersection_pt, surfaces, light)) {
                    c = compute_shading(surface, ray, intersection_pt, light);
                    px.r += c.red;
                    px.g += c.green;
                    px.b += c.blue;
                }
            }
        }
    }

    printf("Generating image: %s\n", output_file);
    exr::writeRgba(output_file, &pixels[0][0],
                   camera.pixelsX(), camera.pixelsY());

    /* cleanup up surfaces */
    cleanup(surfaces, lights);
}
