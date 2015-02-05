#include "Ray.h"


size_t Ray::counter = 0;
Ray::Ray(Pos startPosition, Vec3f direction)
:_id(++counter), startPosition(startPosition), direction(direction.normalize())
{
    t_max = INFINITY;
    color = BACKGROUND_COLOR;
}

Colr Trace(int bounces){
	return Colr(1, 1, 1);
}