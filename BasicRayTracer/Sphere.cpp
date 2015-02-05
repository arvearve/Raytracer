#include "Sphere.h"

Sphere::Sphere(Pos center, float radius, Colr diffuse)
:center(center), radius(radius), diffuse(diffuse), radius_sq(radius*radius)
{}

Sphere::Sphere(const SphereIO data, const MaterialIO material):
center(Pos(data.origin)),
radius(data.radius),
diffuse(Colr(material.diffColor))
{}

void Sphere::intersect(Ray &ray) const{
    Vec3f L = ray.startPosition - center;
    float a = Vec3f::dot(ray.direction, ray.direction);
    float b = Vec3f::dot(ray.direction, L) * 2;
    float c = Vec3f::dot(L, L) - radius_sq;

    float discriminant = b*b - 4 * a * c;
    if (discriminant < 0) {
        // No intersection
        return;
    }
    float t = quadratic(a, b, discriminant);
    if(t > ray.t_max){ return; }
    ray.t_max = t;
    ray.color = diffuse;
}

float Sphere::quadratic(const float a, const float b, const float discriminant) const {
    return (-b - sqrt(discriminant)) / 2*a;
}