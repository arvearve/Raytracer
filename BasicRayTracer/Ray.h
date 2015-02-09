#pragma once
#ifndef __RAY_H
#define __RAY_H
#include <vector>
#include <math.h>
#include <unordered_set>
#include "Vec3f.h"
#include "scene_io.h"
class Primitive;

#define BACKGROUND_COLOR Colr(0,0,0)
#define BUMP_EPSILON 0.0001
#define IOR_AIR 1.0
#define IOR_GLASS 1.5
class Ray
{

private:
    bool isReflective(MaterialIO material);
public:
	static size_t counter;
	size_t _id;

    Pos startPosition;
    Vec3f direction;

    Colr color;
    float t_max;
    MaterialIO material;
    Vec3f intersectionNormal;

    Primitive* currentObject;
    std::unordered_set<Primitive*> insideObjects;

    Pos intersectionPoint();
    Colr trace(int bounces);
    Colr diffuse(const Pos point) const;
    Colr specular(const Pos point) const;
    Colr ambient() const;
    Colr reflection(const Pos point, const int bounces) const;
    Colr refraction(const Pos point, const int bounces, float n1, float n2);
    Colr shadow(const Pos point, const LightIO light) const;
    float attenuationFactor(const Pos point, const LightIO* light) const;
    Ray(Pos startPosition, Vec3f direction);

};

#endif