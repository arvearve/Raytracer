//
//  box_triangle.h
//  BasicRayTracer
//
//  Created by Arve Nygård on 25/02/15.
//  Copyright (c) 2015 Arve Nygård. All rights reserved.
//

#ifndef __BasicRayTracer__box_triangle__
#define __BasicRayTracer__box_triangle__
#include "Vec3f.h"
#include "scene_io.h"
#include "Ray.h"
#include <stdio.h>
struct Box {
    Vec3f min;
    Vec3f max;
    Box(const Vec3f min, const Vec3f max): min(min), max(max){};
    Box(){};
    std::pair<float, float> intersect(Ray &r) const;
    Vec3f bounds(int type) const{
        if(type == 0) return min;
        return max;
    }
    float dx() const {
        return max.x - min.x;
    };
    float dy() const {
        return max.y - min.y;
    };
    float dz() const {
        return max.z - min.z;
    };
    float d(int axis) const {
        return max[axis] - min[axis];
    }
    float isPlanar(){
        return dx() <= 0.01 || dy() <= 0.01 || dz() <= 0.01;
    }
};
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
    Box bounds;
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
        bounds = getBounds();
        uu = Vec3f::dot(u,u);
        uv = Vec3f::dot(u,v);
        vv = Vec3f::dot(v,v);
        invDenom = 1.0 / (uv*uv - uu*vv);
        n = Vec3f::cross(v, u);
    }

    bool intersect(Ray &ray) const;
    MaterialIO interpolate(const float u,const float v,const VertexIO &v1, const VertexIO &v2, const VertexIO &v3) const;
    Vec3f interpNormals(const float u, const float v, const Vec3f &n0, const Vec3f &n1, const Vec3f &v2) const;

    float leftExtreme(int axis){
        return fmin(p0[axis], fmin(p1[axis], p2[axis]));
    }
    float rightExtreme(int axis){
        return fmax(p0[axis], fmax(p1[axis], p2[axis]));
    }

    Box getBounds(){
        float xmin = fmin(p0.x, fmin(p1.x, p2.x));
        float ymin = fmin(p0.y, fmin(p1.y, p2.y));
        float zmin = fmin(p0.z, fmin(p1.z, p2.z));

        float xmax = fmax(p0.x, fmax(p1.x, p2.x));
        float ymax = fmax(p0.y, fmax(p1.y, p2.y));
        float zmax = fmax(p0.z, fmax(p1.z, p2.z));


        return Box(Vec3f(xmin, ymin, zmin), Vec3f(xmax, ymax, zmax));
    }
    float area();
};

#endif /* defined(__BasicRayTracer__box_triangle__) */
