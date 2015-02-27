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
#include "box_triangle.h"
#include "kdTree.h"

class Mesh;

class Mesh : public Primitive {
public:
    Node node;
    std::vector<Triangle*> triangles;
    std::vector<Vec3f> normals;
    std::vector<MaterialIO> materials;
    long triangleCount;
    MaterialBinding materialBinding;
    NormType normType;
    Mesh(const PolySetIO polySet, const MaterialIO* materials, const long materialCount, char* name);

    virtual bool intersect(Ray &ray);
    Vec3f normal(const PolygonIO polygon) const;

};
#endif /* defined(__BasicRayTracer__Mesh__) */
