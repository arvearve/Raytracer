//
//  PhotonMap.h
//  BasicRayTracer
//
//  Created by Arve Nygård on 19/03/15.
//  Copyright (c) 2015 Arve Nygård. All rights reserved.
//

#ifndef __BasicRayTracer__PhotonMap__
#define __BasicRayTracer__PhotonMap__
#include "Vec3f.h"
#include <vector>
#include <stdio.h>
#include <queue>
#include <algorithm>
#include "box_triangle.h"

typedef struct Photon{
    Photon(Pos position, Vec3f incidentDirection, Colr flux): position(position), incidentDirection(incidentDirection*-1), flux(flux){};
    Photon(float x, float y, float z):position(Vec3f(x,y,z)){};
    Pos position;
    Vec3f incidentDirection;
    Colr flux;
    int plane;
} Photon;

typedef struct PNode {
    PNode(Photon *p, PNode *l, PNode *r):photon(p), left(l), right(r){};
    Photon* photon;
    PNode* left;
    PNode* right;
} PNode;

struct Result {
    Result(Photon *photon, const float dx): photon(photon), dx(dx){};
    Result(){}
    bool operator<(const Result &result) const {
        return dx < result.dx;
    }
    Photon *photon;
    float dx;
};

class PhotonMap {
private:
    bool comparePhotons(const Photon& left, const Photon&right) const;
    std::vector<Photon> photons;
    PNode* kdtree;
    void nearest(const PNode *root, const Pos &query, int &visited, const int k, std::priority_queue<Result> &heap);
public:
    PhotonMap():photons(std::vector<Photon>()){}
    void store(const Photon &photon);
    void build();
    PNode* buildTree(std::vector<Photon*> photonList, int depth);
    std::priority_queue<Result> kNN(const Pos position, const int k);

};

#endif /* defined(__BasicRayTracer__PhotonMap__) */
