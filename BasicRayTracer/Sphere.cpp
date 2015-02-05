#include "Sphere.h"

Sphere::Sphere(Pos center, float radius, Colr diffuse)
:center(center), radius(radius), material(material)
{
    radius_sq = radius*radius;
}

Sphere::Sphere(const SphereIO data, const MaterialIO material):
center(Pos(data.origin)),
radius(data.radius),
material(material)
{
    radius_sq = radius*radius;
}

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
    if(t > ray.t_max){
        // This ray has already intersected a sphere at a closer point. Ignore this sphere.
        return;
    }
    ray.t_max = t;
    ray.color = diffuse(ray.intersectionPoint());
}


Colr Sphere::diffuse(const Pos point) const{
    Colr result = Colr(0,0,0);
    extern SceneIO *scene;
    LightIO *light = scene->lights;
    while (light != NULL){
        Vec3f directionToLight = (Pos(light->position) - point).normalize();
        light = light->next;
        result = result + Colr(material.diffColor) * fmax(0,Vec3f::dot(directionToLight,normal(point)));
        result = result * (1.0/sqrt(3));
    }
    return result;
}



Vec3f Sphere::normal(const Pos point) const{
    return Vec3f::normalize(Vec3f(point - center));
}

float Sphere::quadratic(const float a, const float b, const float discriminant) const {
    return (-b - sqrt(discriminant)) / 2*a;
}