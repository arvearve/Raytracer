//
//  Mesh.cpp
//  BasicRayTracer
//
//  Created by Arve Nygård on 08/02/15.
//  Copyright (c) 2015 Arve Nygård. All rights reserved.
//

#include "Mesh.h"
#define EPSILON 0.0001f
Mesh::Mesh(const PolySetIO polySet, const MaterialIO material):material(material), triangleCount(polySet.numPolys){
    if(polySet.type != POLYSET_TRI_MESH){ std::cout << "Unimplemented polyset type: " << polySet.type << std::endl; }
    if(polySet.normType != PER_FACE_NORMAL){ std::cout << "Unimplemented normal type: " << polySet.normType << std::endl; }
    if(polySet.materialBinding != PER_OBJECT_MATERIAL){ std::cout << "Unimplemented material binding: " << polySet.materialBinding << std::endl; }

    for(int i = 0; i < polySet.numPolys; i++){
        PolygonIO polygon = polySet.poly[i];
        normals.push_back(normal(polygon));
        for (int v = 0; v < polygon.numVertices; v++) {
            vertices.push_back(polygon.vert[v]);
        }

    }
}


bool Mesh::intersect(Ray &ray){
    bool result = false;
    for (int i = 0; i < triangleCount ; i++) {
        int offset = i * 3;
        Pos v0 = Pos(vertices[offset].pos);
        Pos v1 = Pos(vertices[offset+1].pos);
        Pos v2 = Pos(vertices[offset+2].pos);
        Vec3f N = normals[i];
        result = result || intersectTriangle(ray, v0, v1, v2, N);
    }
    return result;
}

bool Mesh::intersectTriangle(Ray &ray, const Pos v0, const Pos v1, const Pos v2, const Vec3f N){
    Vec3f u = v1 - v0;
    Vec3f v = v2 - v0;

    // plane intersect
    float r1 = Vec3f::dot(N, v0 - ray.startPosition);
    float r2 = Vec3f::dot(N, ray.direction);

    if (fabs(r2) < 0.0000001) { return false; }
    float r = r1 / r2;
    if (r <= 0.00001) { return false; }

    // Intersection point with Plane
    Pos intersectionPoint = ray.startPosition + ray.direction * r;
    Vec3f w = intersectionPoint - v0;
    // Now test if we hit inside the triangle
    float uu = Vec3f::dot(u, u);
    float uv = Vec3f::dot(u, v);
    float uw = Vec3f::dot(u, w);
    float vv = Vec3f::dot(v, v);
    float vw = Vec3f::dot(v, w);

    float denom = uv*uv - uu*vv;
    float s = (uv*vw - vv*uw) / denom;
    float t = (uv*uw - uu*vw) / denom;

    // triangle test
    if (s < 0.0f || t < 0.0f || s + t > 1.0f)
    { return false; }

    // hit found

    if (r < ray.t_max)
    {
        ray.currentObject = this;
        ray.t_max= r;
        ray.intersectionNormal = Vec3f::normalize(N);
        ray.material = material;
        return true;
    }
    return false;
}

Vec3f Mesh::normal(const PolygonIO polygon) const {
    Pos a = Pos(polygon.vert[0].pos);
    Pos b = Pos(polygon.vert[1].pos);
    Pos c = Pos(polygon.vert[2].pos);
    Vec3f ab = b-a;
    Vec3f ac = c-a;
    Vec3f N = Vec3f::cross(ab, ac);
    return N;
}



