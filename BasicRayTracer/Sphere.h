#ifndef _SPHERE_H
#define _SPHERE_H

#include "Primitive.h"
#include "scene_io.h"
class Sphere : public Primitive
{
public:
	Pos center;
    MaterialIO material;
	float radius;
    float radius_sq;
	Sphere(Pos center, float radius, Colr diffuse);
    Sphere(const SphereIO data, const MaterialIO material);

    void intersect(Ray &ray) const;
    Colr diffuse(const Pos point) const;
    Vec3f normal(const Pos point) const;
	float discriminant(const float a, const float b, const float c) const;
    float quadratic(const float a, const float b, const float discriminant) const;
};

#endif