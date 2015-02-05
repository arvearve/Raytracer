#pragma once
#include "Vec3f.h"
#define BACKGROUND_COLOR Colr(0.5,0.5,0.5)
class Ray
{
public:
	static size_t counter;
	size_t _id;
    float t_max;
    Colr color;
    Pos startPosition;
	Vec3f direction;
	Colr Trace(int bounces);

    Ray(Pos startPosition, Vec3f direction);

};
