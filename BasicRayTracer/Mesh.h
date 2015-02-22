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
    std::vector<PolygonIO> polygons;
    std::vector<Vec3f> normals;
    std::vector<MaterialIO> materials;
    long triangleCount;
    MaterialBinding materialBinding;
    NormType normType;


    MaterialIO interpolate(const float u,const float v,const VertexIO &v1, const VertexIO &v2, const VertexIO &v3) const;
    Vec3f interpNormals(const float u, const float v, const VertexIO &v0, const VertexIO &v1, const VertexIO &v2) const;

public:
    Mesh(const PolySetIO polySet, const MaterialIO* materials, const long materialCount);

    virtual bool intersect(Ray &ray);
    Vec3f normal(const PolygonIO polygon) const;

};
#endif /* defined(__BasicRayTracer__Mesh__) */
