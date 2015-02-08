#ifndef _PRIMITIVE_H
#define _PRIMITIVE_H

#include "Ray.h"
class Primitive {
public:
    MaterialIO material;

	Primitive();
	~Primitive();
	virtual void intersect(Ray &ray) const = 0;
    virtual Vec3f normal(const Pos point) const = 0;
};

#endif