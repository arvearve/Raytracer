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
    bool isReflective() const;
    bool isTransparent() const;
public:
	static size_t counter;
	size_t _id;

    Pos startPosition;
    Vec3f direction;

    Colr color;
    float t_max;
    float u;
    float v;
    MaterialIO material;
    Vec3f intersectionNormal;

    Primitive* currentObject;
    
    Pos intersectionPoint() const;
    Colr trace(int bounces, std::unordered_set<Primitive*> insideObjects);
    Colr trace(int bounces);
    Colr diffuse(const Vec3f &L, Colr &color) const;
    Colr specular(const Vec3f &L, Colr &color) const;
    Colr ambient() const;

    Colr reflection(const Pos point, const int bounces, std::unordered_set<Primitive*> mySet) const;
    Colr refraction(const Pos point, const int bounces, const float ior_a, const float ior_b, std::unordered_set<Primitive*> mySet, std::unordered_set<Primitive*> oldSet);

    Colr shadow(const Vec3f &L, const float lightDistance) const;
    float attenuationFactor(const Pos point, const LightIO* light) const;
    Ray(Pos startPosition, Vec3f direction);

};

#endif