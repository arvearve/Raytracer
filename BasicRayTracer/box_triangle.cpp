//
//  box_triangle.cpp
//  BasicRayTracer
//
//  Created by Arve Nygård on 25/02/15.
//  Copyright (c) 2015 Arve Nygård. All rights reserved.
//

#include "box_triangle.h"

std::pair<float, float> Box::intersect(Ray &r) const{
    float tmin, tmax, tymin, tymax, tzmin, tzmax;
    tmin = (bounds(r.sign[0]).x - r.startPosition.x) * r.inv_direction.x;
    tmax = (bounds(1-r.sign[0]).x - r.startPosition.x) * r.inv_direction.x;
    tymin = (bounds(r.sign[1]).y - r.startPosition.y) * r.inv_direction.y;
    tymax = (bounds(1-r.sign[1]).y - r.startPosition.y) * r.inv_direction.y;
    if ((tmin > tymax) || (tymin > tmax)){
        return std::pair<float,float>(0, 0);
    }
    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;
    tzmin = (bounds(r.sign[2]).z - r.startPosition.z) * r.inv_direction.z;
    tzmax = (bounds(1-r.sign[2]).z - r.startPosition.z) * r.inv_direction.z;
    if ((tmin > tzmax) || (tzmin > tmax)){
        return std::pair<float,float>(0, 0);
    }
    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;
    return  std::pair<float,float>(tmin, tmax);
}