#include "Sphere.h"

Sphere::Sphere(const SphereIO data, const MaterialIO material):
center(Pos(data.origin)),
radius(data.radius),
xAxis(Vec3f(data.xaxis).normalize()),
yAxis(Vec3f(data.yaxis).normalize()),
zAxis(Vec3f(data.zaxis).normalize()),
material(material)
{
    radius_sq = radius*radius;
}

bool Sphere::intersect(Ray &ray) {
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
    if(t > ray.t_max || t < 0){
        // This ray has already intersected another primitive at a closer point. Ignore this sphere.
        return false;
    }

    ray.currentObject = this;
    ray.t_max = t;
    ray.material = material;
    ray.intersectionNormal = normal(ray.intersectionPoint());
    uv(ray.intersectionNormal, ray.u, ray.v);
    return true;
}

void Sphere::uv(const Vec3f &normal, float &u, float &v) const{
    float phi = acos(-Vec3f::dot(yAxis, normal));
    v = phi / M_PI;

    float theta = (acosf(Vec3f::dot(normal, zAxis) / sin(phi)) ) / (2.0*M_PI);
    if( Vec3f::dot(Vec3f::cross(yAxis, zAxis), normal) > 0.0 ) {
        u = theta;
    } else {
        u = 1.0-theta;
    }

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