//
//  Mesh.cpp
//  BasicRayTracer
//
//  Created by Arve Nygård on 08/02/15.
//  Copyright (c) 2015 Arve Nygård. All rights reserved.
//

#include "Mesh.h"
#define EPSILON 0.00001f
Mesh::Mesh(const PolySetIO polySet, const MaterialIO* materials, const long numMaterials): materials(materials, materials + numMaterials), triangleCount(polySet.numPolys){
    if(polySet.type != POLYSET_TRI_MESH){ std::cout << "Unimplemented polyset type: " << polySet.type << std::endl; }
    float numPolys = polySet.numPolys;
    float xmin = INFINITY, ymin = INFINITY, zmin = INFINITY;
    float xmax = -INFINITY, ymax = -INFINITY, zmax = -INFINITY;

    for(int i = 0; i < numPolys; i++){
        PolygonIO polygon = polySet.poly[i];
        Vec3f N = normal(polygon);
        normals.push_back(N);
        normType = polySet.normType;
        materialBinding = polySet.materialBinding;
        Triangle t = Triangle(polygon, *this, N);
        triangles.push_back(t);

        xmin = fmin(xmin, fmin(polygon.vert[0].pos[0], fmin(polygon.vert[1].pos[0], polygon.vert[2].pos[0])));
        ymin = fmin(ymin, fmin(polygon.vert[0].pos[1], fmin(polygon.vert[1].pos[1], polygon.vert[2].pos[1])));
        zmin = fmin(zmin, fmin(polygon.vert[0].pos[2], fmin(polygon.vert[1].pos[2], polygon.vert[2].pos[2])));

        xmax = fmax(xmax, fmax(polygon.vert[0].pos[0], fmax(polygon.vert[1].pos[0], polygon.vert[2].pos[0])));
        ymax = fmax(ymax, fmax(polygon.vert[0].pos[1], fmax(polygon.vert[1].pos[1], polygon.vert[2].pos[1])));
        zmax = fmax(zmax, fmax(polygon.vert[0].pos[2], fmax(polygon.vert[1].pos[2], polygon.vert[2].pos[2])));

    }


    bounds[0] = Vec3f(xmin, ymin, zmin);
    bounds[1] = Vec3f(xmax, ymax, zmax);
}


bool Mesh::intersect(Ray &ray){
    if(!bboxIntersect(ray)){ return false; }
    for (int i = 0; i < triangleCount ; i++) {
        triangles[i].intersect(ray);
    }
    // Triangle Intersection!
    return ray.t_max < INFINITY;
}

bool Triangle::intersect(Ray &ray) const{
    // First check for plane intersection
    Vec3f w0 = p0 - ray.startPosition;
    float r1 = Vec3f::dot(n, w0);
    float r2 = Vec3f::dot(n, ray.direction);

    // Check if the ray is parallel to the plane (within some epsilon)
    if(fabsf(r2) <= EPSILON) {
        return false;
    }
    float r = r1 / r2;

    if( r < 0.0) { return false; }
    if ( r > ray.t_max) { return false; }

    // Plane intersection confirmed. Check if we are inside the triangle:

    Pos p1 = ray.startPosition + ray.direction * r;
    Pos w = p1 - p0;
    float uw = Vec3f::dot(u, w);
    float vw = Vec3f::dot(v, w);
    float s = (uv * vw - vv*uw) * invDenom;
    float t = (uv * uw - uu*vw) * invDenom;
    if (s < 0.0f || t < 0.0f || s + t > 1.0f){ return false; } // Outside triangle
                                                               // Triangle Intersection!
    if (r < ray.t_max) {
        ray.currentObject = &parentMesh;
        ray.t_max= r;
        ray.u = s;
        ray.v = t;
        if(parentMesh.normType == PER_VERTEX_NORMAL){
            ray.intersectionNormal = interpNormals(s, t, n0, n1, n2);
        } else {
            ray.intersectionNormal = provided_n;
        }
        if(parentMesh.materialBinding == PER_VERTEX_MATERIAL){
            ray.material = interpolate(s, t, v0, v1, v2);
        } else {
            ray.material = parentMesh.materials[0];
        }
    }

    return true;

}
Vec3f Triangle::interpNormals(const float u, const float v, const Vec3f &n1, const Vec3f &n2, const Vec3f &n3) const {
    float w = 1.0 - (u+v);
    return n2*u + n3*v + n1*w;
}
MaterialIO Triangle::interpolate(const float u,const float v,const VertexIO &v1, const VertexIO &v2, const VertexIO &v3) const {
    MaterialIO result;
    MaterialIO m1 = parentMesh.materials[v1.materialIndex];
    MaterialIO m2 = parentMesh.materials[v2.materialIndex];
    MaterialIO m3 = parentMesh.materials[v3.materialIndex];
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



