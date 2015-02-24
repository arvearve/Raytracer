//
//  Mesh.cpp
//  BasicRayTracer
//
//  Created by Arve Nygård on 08/02/15.
//  Copyright (c) 2015 Arve Nygård. All rights reserved.
//

#include "Mesh.h"
#define EPSILON 0.0001f
Mesh::Mesh(const PolySetIO polySet, const MaterialIO* materials, const long numMaterials): materials(materials, materials + numMaterials), triangleCount(polySet.numPolys){
    if(polySet.type != POLYSET_TRI_MESH){ std::cout << "Unimplemented polyset type: " << polySet.type << std::endl; }
    float numPolys = polySet.numPolys;

    for(int i = 0; i < numPolys; i++){
        PolygonIO polygon = polySet.poly[i];
        Vec3f N = normal(polygon);
        normals.push_back(N);
        normType = polySet.normType;
        materialBinding = polySet.materialBinding;
        polygons.push_back(polygon);
    }
}


bool Mesh::intersect(Ray &ray){
    for (int i = 0; i < triangleCount ; i++) {
        PolygonIO poly = polygons[i];
        VertexIO v0 = poly.vert[0];
        VertexIO v1 = poly.vert[1];
        VertexIO v2 = poly.vert[2];
        Vec3f N = normals[i];
        Vec3f v0_Origin = Vec3f(v0.pos[0] - ray.startPosition.x,
                                v0.pos[1] - ray.startPosition.y,
                                v0.pos[2] - ray.startPosition.z);
        // u = v1 - v0
        Vec3f u = Vec3f(v1.pos[0] - v0.pos[0],
                        v1.pos[1] - v0.pos[1],
                        v1.pos[2] - v0.pos[2]);

        // v = v2 - v0;
        Vec3f v = Vec3f(v2.pos[0] - v0.pos[0],
                        v2.pos[1] - v0.pos[1],
                        v2.pos[2] - v0.pos[2]);

        // plane intersect
        float r1 = Vec3f::dot(N, v0_Origin);
        float r2 = Vec3f::dot(N, ray.direction);

        if (fabs(r2) < EPSILON) { continue; }
        float r = r1 / r2;
        if (r <= EPSILON) { continue; }

        // Intersection point with Plane
        Pos intersectionPoint = Pos(ray.startPosition.x + ray.direction.x * r,
                                    ray.startPosition.y + ray.direction.y * r,
                                    ray.startPosition.z + ray.direction.z * r);

        Vec3f w = Vec3f(intersectionPoint.x - v0.pos[0],
                        intersectionPoint.y - v0.pos[1],
                        intersectionPoint.z - v0.pos[2]);
        // Now test if we hit inside the triangle
        float uu = Vec3f::dot(u, u);
        float uv = Vec3f::dot(u, v);
        float uw = Vec3f::dot(u, w);
        float vv = Vec3f::dot(v, v);
        float vw = Vec3f::dot(v, w);

        float denom = uv*uv - uu*vv;
        if( denom >= -EPSILON){ continue; }
        float s = (uv*vw - vv*uw) / denom;
        float t = (uv*uw - uu*vw) / denom;

        // triangle test
        if (s <= 0.0f || t <= 0.0f || s + t > 1.0f) { continue; }

        // hit found
        if (r < ray.t_max) {
            ray.currentObject = this;
            ray.t_max= r;
            ray.u = s;
            ray.v = t;
            if(normType == PER_VERTEX_NORMAL){
                ray.intersectionNormal = interpNormals(s, t, v0, v1, v2);
            } else {
                ray.intersectionNormal = normals[i];
            }

            if(materialBinding == PER_VERTEX_MATERIAL){
                ray.material = interpolate(s, t, v0, v1, v2);
            } else {
                ray.material = materials[0];
            }

        }
    }
    return ray.t_max < INFINITY;
}


Vec3f Mesh::interpNormals(const float u, const float v, const VertexIO &v1, const VertexIO &v2, const VertexIO &v3) const {
    Vec3f n1 = Vec3f(v1.norm);
    Vec3f n2 = Vec3f(v2.norm);
    Vec3f n3 = Vec3f(v3.norm);
    float w = 1.0 - (u+v);
    return n2*u + n3*v + n1*w;
}
MaterialIO Mesh::interpolate(const float u,const float v,const VertexIO &v1, const VertexIO &v2, const VertexIO &v3) const {
    MaterialIO result;
    MaterialIO m1 = materials[v1.materialIndex];
    MaterialIO m2 = materials[v2.materialIndex];
    MaterialIO m3 = materials[v3.materialIndex];
    float w = 1.0 - (u+v);

    result.ktran = u*m2.ktran + v*m3.ktran + w*m1.ktran;
    result.shininess = u*m2.shininess + v*m3.shininess + w*m1.shininess;

    result.diffColor[0] = u*m2.diffColor[0] + v*m3.diffColor[0] + w*m1.diffColor[0];
    result.diffColor[1] = u*m2.diffColor[1] + v*m3.diffColor[1] + w*m1.diffColor[1];
    result.diffColor[2] = u*m2.diffColor[2] + v*m3.diffColor[2] + w*m1.diffColor[2];

    result.ambColor[0] = u*m2.ambColor[0] + v*m3.ambColor[0] + w*m1.ambColor[0];
    result.ambColor[1] = u*m2.ambColor[1] + v*m3.ambColor[1] + w*m1.ambColor[1];
    result.ambColor[2] = u*m2.ambColor[2] + v*m3.ambColor[2] + w*m1.ambColor[2];

    result.emissColor[0] = u*m2.emissColor[0] + v*m3.emissColor[0] + w*m1.emissColor[0];
    result.emissColor[1] = u*m2.emissColor[1] + v*m3.emissColor[1] + w*m1.emissColor[1];
    result.emissColor[2] = u*m2.emissColor[2] + v*m3.emissColor[2] + w*m1.emissColor[2];

    result.specColor[0] = u*m2.specColor[0] + v*m3.specColor[0] + w*m1.specColor[0];
    result.specColor[1] = u*m2.specColor[1] + v*m3.specColor[1] + w*m1.specColor[1];
    result.specColor[2] = u*m2.specColor[2] + v*m3.specColor[2] + w*m1.specColor[2];

    return result;
}




Vec3f Mesh::normal(const PolygonIO polygon) const {
    Pos a = Pos(polygon.vert[0].pos);
    Pos b = Pos(polygon.vert[1].pos);
    Pos c = Pos(polygon.vert[2].pos);
    Vec3f ab = b-a;
    Vec3f ac = c-a;
    Vec3f N = Vec3f::cross(ab, ac).normalize();
    return N;
}



