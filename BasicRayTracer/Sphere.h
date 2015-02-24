#ifndef _SPHERE_H
#define _SPHERE_H

#include "Primitive.h"
#include "scene_io.h"
class Sphere : public Primitive
{
private:
	Pos center;
    Vec3f xAxis;
    Vec3f yAxis;
    Vec3f zAxis;

    float radius;
    float radius_sq;
    float discriminant(const float a, const float b, const float c) const;
    float quadratic_min(const float a, const float b, const float discriminant) const;
    float quadratic_max(const float a, const float b, const float discriminant) const;

    void uv(const Vec3f &normal, float &u, float &v) const;
public:
    MaterialIO material;
    Sphere(const SphereIO data, const MaterialIO material, char* _name);

    virtual bool intersect(Ray &ray);
    Vec3f normal(const Pos point) const;
};

#endif