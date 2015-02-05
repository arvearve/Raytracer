#pragma once
#include "Vec3f.h"
#include "scene_io.h"
#define BACKGROUND_COLOR Colr(0.5,0.5,0.5)
class Ray
{
public:
	static size_t counter;
	size_t _id;

    Pos startPosition;
    Vec3f direction;

    Colr color;
    float t_max;


    Pos intersectionPoint();
    Colr trace(int bounces);

    Ray(Pos startPosition, Vec3f direction);

};
