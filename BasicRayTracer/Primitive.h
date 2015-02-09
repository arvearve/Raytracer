#ifndef _PRIMITIVE_H
#define _PRIMITIVE_H

#include "Ray.h"
class Primitive {
public:
    MaterialIO material;

	Primitive();
	~Primitive();
	virtual bool intersect(Ray &ray) = 0;
};

#endif