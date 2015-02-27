#include "Sphere.h"
extern bool CHECKERBOARD(const float u, const float v);
Sphere::Sphere(const SphereIO data, const MaterialIO material, char* _name):
center(Pos(data.origin)),
radius(data.radius),
xAxis(Vec3f(data.xaxis).normalize()),
yAxis(Vec3f(data.yaxis).normalize()),
zAxis(Vec3f(data.zaxis).normalize()),
material(material)
{
    name = _name;
    radius_sq = radius*radius;
    bounds[0] = Vec3f(center.x - radius, center.y - radius, center.z - radius);
    bounds[1] = Vec3f(center.x + radius, center.y + radius, center.z + radius);
}

bool Sphere::intersect(Ray &ray) {
    Vec3f L = ray.startPosition - center;
    // a is always 1 :D
    float b = Vec3f::dot(ray.direction, L) * 2.0;
    float c = Vec3f::dot(L, L) - radius_sq;

    float discriminant = b*b - 4 * c;
    if (discriminant < 0) {
        // No intersection
        return false;
    }
    float t, t_min, t_max;
    if(discriminant == 0){
        // Tangent single hit
        t = -b * 0.5;
    }
    else{
        // Two intersections
        auto sqrt = sqrtf(discriminant);
        auto t1 = (-b + sqrt) / 2.0f;
        auto t2 = (-b - sqrt) / 2.0f;
        t_min = fmin(t1, t2);
        t_max = fmax(t1, t2);
    }

    if(t_min > ray.t_max || t_min < 0){
        // This ray has already intersected another primitive at a closer point. Ignore this sphere.
        return false;
    }

    // We hit this object.

    /* Handle intersection shader. If no shader is set, we can use the nearest intersection point immediately.
      Shaders are stored in the name parameter.
     */

    Vec3f intersectionNormal = normal(ray.startPosition + ray.direction * t_min);
    t = t_min;
    float u, v;
    if(name != NULL){
        // Intersection shader is set. We need uv coordinates, and possibly two interseciton checks.

        uv(intersectionNormal, u, v);
        if(atoi(name)/10 == 1){ // Checkerboard is a 2-digit name starting with '1'.
            if(!CHECKERBOARD(u, v)){
                // Try again with far intersection
                t = t_max;
                intersectionNormal = normal(ray.startPosition + ray.direction * t_max);
                uv(intersectionNormal, u, v);
                if(!CHECKERBOARD(u, v)){ return false;}
            }
        }

    }

    ray.t_max = t;
    ray.intersectionNormal = intersectionNormal;
    ray.currentObject = this;
    ray.material = material;
    ray.u = u;
    ray.v = v;
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
    return (-b - sqrt(discriminant)) / 2;
}
float Sphere::quadratic_max(const float a, const float b, const float discriminant) const {
    return (-b + sqrt(discriminant)) / 2;
}