#include "point_light.h"

PointLight::PointLight(float x, float y, float z, float r, float g, float b)
{
    c = {.red = r, .green = g, .blue = b};
    position = {.x = x, .y = y, .z = z};
    intensity = 1;
}
/**
 * Checks whether a point on a surface is
 * occluded by any other surface between the point
 * and the light. Used for computing shadows.
 *
 * @param point - point of intersection
 * @param surfaces - a vector of surfaces in the scene
 * @param light - a point light from which the occlusion has
 * to be calculated
 * @return true to indicate if the surface is occluded.
 */
bool PointLight::is_occluded_by (
        const Raytra::point& point,
        const std::vector<Surface*>& surfaces,
        const BVHTree* tree
)
{
    /* to avoid shadow rounding errors */
    const float surface_delta = 0.001;

    /* generate ray from point to light */
    vec light_dir = norm(position - point);
    Ray light_ray(point, light_dir);

    /* the t at which the light is located */
    float t_light = light_ray.offset(position);

    std::vector<int> surface_indices;
    tree->compute_intersections(light_ray, surface_indices);

    /* compute intersection of light ray with all surfaces */
    for (int i: surface_indices) {
        Surface* surface = surfaces[i];
        float t = surface->get_intersection_point(light_ray);
        if (t > surface_delta && t < t_light &&
            fabsf(t) > surface_delta)
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
 *
 * @Returns - a color (r,g,b triple) to denote the shading at the point
 */
color PointLight::compute_shading (
        const Surface *surface,
        const Ray &camera_ray,
        const Raytra::point &point
)
{
    vec surface_normal = surface->get_normal(point);
    const vec light_ray = norm(position - point);
    const vec bisector = norm(-camera_ray.dir + light_ray);
    const float d2 = dist2(position, point);


    color kd, ks;
    if (surface->is_front_facing(camera_ray)) {
        kd = surface->material->diffuse;
        ks = surface->material->specular;
    } else {
        kd = { .red = 1, .green = 1, .blue = 0 };
        ks = { .red = 0, .green = 0, .blue = 0 };
        surface_normal = -surface_normal;
    }

    const float cosine = fmaxf(0, dot(surface_normal, light_ray));
    const color diffuse = {
            .red = kd.red * c.red * cosine * intensity,
            .green = kd.green * c.green * cosine * intensity,
            .blue = kd.blue * c.blue * cosine * intensity,
    };

    const float cosalpha = fmaxf(0, dot(surface_normal, bisector));
    const float multiplier = intensity * powf(cosalpha, surface->material->phong);

    const color specular = {
            .red = ks.red * multiplier * c.red,
            .green = ks.green * multiplier * c.green,
            .blue = ks.blue * multiplier * c.blue,
    };

    return {
            .red = (diffuse.red + specular.red)/d2,
            .green = (diffuse.green + specular.green)/d2,
            .blue = (diffuse.blue + specular.blue)/d2,
    };
}
