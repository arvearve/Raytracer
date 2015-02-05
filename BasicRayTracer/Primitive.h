#pragma once
#include "Ray.h"
class Primitive
{
public:
	Primitive();
	~Primitive();
	virtual void intersect(Ray &ray) const = 0;
};

