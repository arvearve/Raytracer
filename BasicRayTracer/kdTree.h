//
//  kdTree.h
//  BasicRayTracer
//
//  Created by Arve Nygård on 24/02/15.
//  Copyright (c) 2015 Arve Nygård. All rights reserved.
// http://www.sci.utah.edu/~wald/PhD/wald_phd.pdf

#ifndef __BasicRayTracer__kdTree__
#define __BasicRayTracer__kdTree__

#include <stdio.h>
#include "box_triangle.h"

struct SplitPlane {
    SplitPlane(const int axis, const float pos): axis(axis), pos(pos){};
    SplitPlane(){};

    int axis; // 0=x, 1=y, 2=z;
    float pos;

    bool operator==(const SplitPlane& sp) {
        return(axis == sp.axis && pos == sp.pos);
    }
};

class Node{
public:
    Node():leaf(false), triangles(std::vector<Triangle*>()){};
    Node *left;
    Node *right;
    bool leaf;
    SplitPlane splitPlane;
    Box bounds;
    std::vector<Triangle*> triangles;
    bool isDone(int N, float minCv) const;
    float getSplitPos();
    void build(Node &node);
    bool intersects(Triangle *triangle);
    void intersectAllTriangles(Ray &r);
    float calculateCost(const float &position);
    void traverse(Ray &r);
    void traverse(Ray &r, float t_near, float t_far);


    void splitBox(const Box& V, const SplitPlane& p, Box& VL, Box& VR) const;
    typedef enum { LEFT=-1, RIGHT=1, UNKNOWN=0 } PlaneSide;
    void SAH(const SplitPlane& p, const Box& V, int NL, int NR, int NP, float& CP, PlaneSide& pside) const;
    void findPlane(const std::vector<Triangle *>& T, const Box& V, int depth, SplitPlane& p_est, float& C_est, PlaneSide& pside_est) const;
    void sortTriangles(const std::vector<Triangle*>& T, const SplitPlane& p, const PlaneSide& pside, std::vector<Triangle*>& TL, std::vector<Triangle*>& TR) const;
    Node* RecBuild(std::vector<Triangle *> T, const Box &V, int depth, const SplitPlane& prev_plane);
};




#endif /* defined(__BasicRayTracer__kdTree__) */
