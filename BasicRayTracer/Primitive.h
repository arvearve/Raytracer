#ifndef _PRIMITIVE_H
#define _PRIMITIVE_H

#include "Ray.h"
class Primitive {
public:
    const char* name;
    Vec3f bounds[2];
	Primitive();
	~Primitive();
    bool bboxIntersect(Ray &ray);
	virtual bool intersect(Ray &ray) = 0;
};

#endif