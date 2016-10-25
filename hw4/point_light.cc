#include "point_light.h"

PointLight::PointLight(float x, float y, float z, float r, float g, float b)
{
    c = {.red = r, .green = g, .blue = b};
    position = {.x = x, .y = y, .z = z};
    intensity = 1;
}

/**
 * Computes specular shading at a surface for a point light
 *
 * @param surface - the surface for which shading has to be computed
 * @param point - the point at which the camera ray and surface intersect
 * @param camera_ray - the camera ray at the point of intersection
 *
 * @Returns - a color (r,g,b triple) to denote the shading at the point
 */
color PointLight::specular_shading (
        const Surface* surface,
        const Ray& camera_ray,
        const Raytra::point& point
)
{
    vec surface_normal = surface->get_normal(point);
    vec light_ray = norm(position - point);
    vec bisector = norm(-camera_ray.dir + light_ray);

    float cosalpha = fmaxf(0, dot(surface_normal, bisector));
    float multiplier = intensity * powf(cosalpha, surface->material->phong);
    color ks = surface->material->specular;
    return {
            .red = ks.red * multiplier * c.red,
            .green = ks.green * multiplier * c.green,
            .blue = ks.blue * multiplier * c.blue,
    };
}

/**
 * Computes diffuse shading at a surface for a point light
 *
 * @param surface - the surface for which shading has to be computed
 * @param point - the point at which the camera ray and surface intersect
 *
 * @Returns - a color (r,g,b triple) to denote the shading at the point
 */
color PointLight::diffuse_shading (
        const Surface* surface,
        const Raytra::point& point
)
{
    // diffuse computation
    vec surface_normal = surface->get_normal(point);
    vec light_ray = norm(position - point);

    float cosine = fmaxf(0, dot(surface_normal, light_ray));
    color kd = surface->material->diffuse;

    return {
            .red   = kd.red * c.red * cosine * intensity,
            .green = kd.green * c.green * cosine * intensity,
            .blue  = kd.blue * c.blue * cosine * intensity,
    };
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
        const std::vector<Surface*>& surfaces
)
{
    /* to avoid shadow rounding errors */
    const float surface_delta = 0.05;

    /* generate ray from point to light */
    vec light_dir = norm(position - point);
    Ray light_ray(point, light_dir);

    /* the t at which the light is located */
    float t_light = light_ray.offset(position);

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
 *
 * @Returns - a color (r,g,b triple) to denote the shading at the point
 */
color PointLight::compute_shading (
        const Surface *surface,
        const Ray &camera_ray,
        const Raytra::point &point
)
{
    const float d2 = dist2(position, point);

    // backside of a one-sided surface. color yellow for debugging
    if (surface->is_one_sided() &&
            dot(surface->get_normal(point), -camera_ray.dir) < 0) {
        printf("hitting on back side\n");
        return { .red = 1, .green = 1, .blue = 0 };
    }

    const color diffuse  = diffuse_shading(surface, point);
    const color specular = specular_shading(surface, camera_ray, point);

    return {
            .red = (diffuse.red + specular.red)/d2,
            .green = (diffuse.green + specular.green)/d2,
            .blue = (diffuse.blue + specular.blue)/d2,
    };
}

