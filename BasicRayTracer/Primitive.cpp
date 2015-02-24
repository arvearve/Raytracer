#include "Primitive.h"

/* Fast bounding box intersection
 http://www.cs.utah.edu/~awilliam/box/box.pdf
 */
bool Primitive::bboxIntersect(Ray &r){
    float tmin, tmax, tymin, tymax, tzmin, tzmax;
    tmin = (bounds[r.sign[0]].x - r.startPosition.x) * r.inv_direction.x;
    tmax = (bounds[1-r.sign[0]].x - r.startPosition.x) * r.inv_direction.x;
    tymin = (bounds[r.sign[1]].y - r.startPosition.y) * r.inv_direction.y;
    tymax = (bounds[1-r.sign[1]].y - r.startPosition.y) * r.inv_direction.y;

    if ((tmin > tymax) || (tymin > tmax)) { return false; }
    if (tymin > tmin) { tmin = tymin; }
    if (tymax < tmax) { tmax = tymax; }

    tzmin = (bounds[r.sign[2]].z - r.startPosition.z) * r.inv_direction.z;
    tzmax = (bounds[1-r.sign[2]].z - r.startPosition.z) * r.inv_direction.z;

    if ((tmin > tzmax) || (tzmin > tmax)) { return false; }
    if (tzmin > tmin) { tmin = tzmin; }
    if (tzmax < tmax) { tmax = tzmax; }
    return  tmax > 0;
}

Primitive::Primitive()
{
}


Primitive::~Primitive()
{
}
