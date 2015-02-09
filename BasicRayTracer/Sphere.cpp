#include "Sphere.h"

Sphere::Sphere(const SphereIO data, const MaterialIO material):
center(Pos(data.origin)),
radius(data.radius),
material(material)
{
    radius_sq = radius*radius;
}

bool Sphere::intersect(Ray &ray) const{
    Vec3f L = ray.startPosition - center;
    float a = Vec3f::dot(ray.direction, ray.direction);
    float b = Vec3f::dot(ray.direction, L) * 2;
    float c = Vec3f::dot(L, L) - radius_sq;

    float discriminant = b*b - 4 * a * c;
    if (discriminant < 0) {
        // No intersection
        return false;
    }
    float t_min = quadratic_min(a, b, discriminant);
    float t = t_min;
//    if(t < 0){
//        t = quadratic_max(a, b, discriminant);
//    }
    if(t > ray.t_max || t < 0){
        // This ray has already intersected a sphere at a closer point. Ignore this sphere.
        return false;
    }
    ray.t_max = t;
    ray.material = material;
    ray.intersectionNormal = normal(ray.intersectionPoint());
    return true;
}

Vec3f Sphere::normal(const Pos point) const{
    return Vec3f::normalize(Vec3f(point - center));
}

float Sphere::quadratic_min(const float a, const float b, const float discriminant) const {
    return (-b - sqrt(discriminant)) / 2*a;
}
float Sphere::quadratic_max(const float a, const float b, const float discriminant) const {
    return (-b + sqrt(discriminant)) / 2*a;
}