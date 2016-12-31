#include "triangle.h"

Triangle::Triangle(float ax, float ay, float az,
                   float bx, float by, float bz,
                   float cx, float cy, float cz) {
    p1 = {.x = ax, .y = ay, .z = az};
    p2 = {.x = bx, .y = by, .z = bz};
    p3 = {.x = cx, .y = cy, .z = cz};

    normal = norm(cross(p2 - p1, p3 - p1));

    box = new BoundingBox (
        fminf(ax, fminf(bx, cx)), fmaxf(ax, fmaxf(bx, cx)),
        fminf(ay, fminf(by, cy)), fmaxf(ay, fmaxf(by, cy)),
        fminf(az, fminf(bz, cz)), fmaxf(az, fmaxf(bz, cz))
    );
}

/* sets the vertex normals for the triangle in the mesh */
void Triangle::set_vertex_normals (
    const vec &v1, const vec &v2, const vec &v3
)
{
    n1 = v1; n2 = v2; n3 = v3; in_mesh = true;
}

BoundingBox* Triangle::get_bounding_box() const
{
    return box;
}

vec Triangle::get_normal(const point& p) const
{
    if (!in_mesh)
        return normal;

    /* If the triangle is in mesh, return its
     * barycentric normal for smooth normal.
     *
     * Using cramer's rule to solve this
     */
    float a = p1.x; float b = p1.y; float c = p1.z;
    float d = p2.x; float e = p2.y; float f = p2.z;
    float g = p3.x; float h = p3.y; float i = p3.z;
    float j = p.x; float k = p.y; float l = p.z;

    float akjb = a * k - j * b;
    float jcal = j * c - a * l;
    float blkc = b * l - k * c;
    float eihf = e * i - f * h;
    float gfdi = g * f - d * i;
    float dheg = d * h - e * g;

    float M = a * eihf + b * gfdi + c * dheg;
    if (M == 0)
        return normal;

    float gamma = - (f * akjb + e * jcal + d * blkc) / M;
    float alpha =  (j * eihf + k * gfdi + l * dheg) / M;
    float beta = (i * akjb + h * jcal + g * blkc) / M;

    return norm(alpha * n1 + beta * n2 + gamma * n3);
}

/* as derived using Cramer's rule */
float Triangle::get_intersection_point(const Ray &ray)
{
    float a = p1.x - p2.x; float b = p1.y - p2.y; float c = p1.z - p2.z;
    float d = p1.x - p3.x; float e = p1.y - p3.y; float f = p1.z - p3.z;
    float g = ray.dir.x; float h = ray.dir.y; float i = ray.dir.z;
    float j = p1.x - ray.origin.x; float k = p1.y - ray.origin.y;
    float l = p1.z - ray.origin.z;

    float akjb = a * k - j * b;
    float jcal = j * c - a * l;
    float blkc = b * l - k * c;
    float eihf = e * i - f * h;
    float gfdi = g * f - d * i;
    float dheg = d * h - e * g;

    float M = a * eihf + b * gfdi + c * dheg;

    float t = - (f * akjb + e * jcal + d * blkc) / M;
    if (t < 0)
        return -1;

    float beta =  (j * eihf + k * gfdi + l * dheg) / M;
    if (beta < 0 || beta > 1)
        return -1;

    float gamma = (i * akjb + h * jcal + g * blkc) / M;
    if (gamma < 0 || 1 - beta < gamma)
        return -1;

    return t;
}

bool Triangle::is_front_facing(const Ray &ray) const
{
    return dot(normal, ray.dir) < 0;
}

void Triangle::print()
{
    std::cout << "Triangle" << std::endl;
}
