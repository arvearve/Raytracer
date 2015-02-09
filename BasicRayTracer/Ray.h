#pragma once
#include <vector>
#include <math.h>
#include "Vec3f.h"
#include "scene_io.h"
#define BACKGROUND_COLOR Colr(0,0,0)
class Ray
{
public:
	static size_t counter;
	size_t _id;

    Pos startPosition;
    Vec3f direction;

    Colr color;
    float t_max;
    MaterialIO material;
    Vec3f intersectionNormal;


    Pos intersectionPoint();
    Colr trace(int bounces);
    Colr diffuse(const Pos point) const;
    Colr specular(const Pos point) const;
    Colr ambient() const;
    Colr reflection(const Pos point, const int bounces) const;
    Colr shadow(const Pos point, const Vec3f pointToLight) const;
    float attenuationFactor(const Pos point, const LightIO* light) const;
    Ray(Pos startPosition, Vec3f direction);

};
