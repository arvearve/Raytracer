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
class Mesh;
class Triangle {
public:
    Pos p0, p1, p2;
    Vec3f n0, n1, n2;
    Vec3f u, v, n, provided_n;
    VertexIO v0, v1, v2;
    Mesh &parentMesh;
    float invDenom, uu, uv, vv;
    PolygonIO poly;
    Triangle(const PolygonIO polygon, Mesh &mesh, Vec3f provided_n):
    parentMesh(mesh),
    v0(polygon.vert[0]),
    v1(polygon.vert[1]),
    v2(polygon.vert[2]),
    provided_n(provided_n),
    n0(Vec3f(polygon.vert[0].norm).normalize()),
    n1(Vec3f(polygon.vert[1].norm).normalize()),
    n2(Vec3f(polygon.vert[2].norm).normalize()),
    p0(Vec3f(polygon.vert[0].pos)),
    p1(Vec3f(polygon.vert[1].pos)),
    p2(Vec3f(polygon.vert[2].pos)),
    u(Vec3f(polygon.vert[1].pos) - Vec3f(polygon.vert[0].pos)),
    v(Vec3f(polygon.vert[2].pos) - Vec3f(polygon.vert[0].pos))
    {
        uu = Vec3f::dot(u,u);
        uv = Vec3f::dot(u,v);
        vv = Vec3f::dot(v,v);
        invDenom = 1.0 / (uv*uv - uu*vv);
        n = Vec3f::cross(v, u);
    }
    bool intersect(Ray &ray) const;
    MaterialIO interpolate(const float u,const float v,const VertexIO &v1, const VertexIO &v2, const VertexIO &v3) const;
    Vec3f interpNormals(const float u, const float v, const Vec3f &n0, const Vec3f &n1, const Vec3f &v2) const;
    
};
class Mesh : public Primitive {
public:

    std::vector<Triangle> triangles;
    std::vector<Vec3f> normals;
    std::vector<MaterialIO> materials;
    long triangleCount;
    MaterialBinding materialBinding;
    NormType normType;
    Mesh(const PolySetIO polySet, const MaterialIO* materials, const long materialCount);

    virtual bool intersect(Ray &ray);
    Vec3f normal(const PolygonIO polygon) const;

};
#endif /* defined(__BasicRayTracer__Mesh__) */
