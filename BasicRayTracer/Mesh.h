//
//  Mesh.h
//  BasicRayTracer
//
//  Created by Arve Nygård on 08/02/15.
//  Copyright (c) 2015 Arve Nygård. All rights reserved.
//

#ifndef __BasicRayTracer__Mesh__
#define __BasicRayTracer__Mesh__

#include <stdio.h>
#include <vector>
#include "Primitive.h"
#include "scene_io.h"

class Mesh : public Primitive {
    std::vector<VertexIO> vertices;
    std::vector<Vec3f> normals;
    MaterialIO material;
    long triangleCount;
    bool intersectPolygon(const PolygonIO polygon, Ray& ray) const;
public:
    Mesh(const PolySetIO polySet, const MaterialIO material);

    virtual bool intersect(Ray &ray) const;
    Vec3f normal(const PolygonIO polygon) const;

};
#endif /* defined(__BasicRayTracer__Mesh__) */
