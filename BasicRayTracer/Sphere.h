#ifndef _SPHERE_H
#define _SPHERE_H

#include "Primitive.h"
#include "scene_io.h"
class Sphere : public Primitive
{
private:
	Pos center;
    float radius;
    float radius_sq;
    float discriminant(const float a, const float b, const float c) const;
    float quadratic(const float a, const float b, const float discriminant) const;

public:

    MaterialIO material;
	Sphere(Pos center, float radius, Colr diffuse);
    Sphere(const SphereIO data, const MaterialIO material);

    virtual void intersect(Ray &ray) const;
    virtual Vec3f normal(const Pos point) const;
};

#endif