//
//  PhotonMap.cpp
//  BasicRayTracer
//
//  Created by Arve Nygård on 19/03/15.

#include "PhotonMap.h"
#define DIMENSIONS 3
void PhotonMap::store(const Photon &photon){
    photons.push_back(photon);
}

/* Turn the list of photons into a KD-tree. Make sure you run this before attempting to *use* the tree. */
void PhotonMap::build(){
    std::vector<Photon*> photonPointerList = std::vector<Photon*>();
    for( Photon &p : photons){
        photonPointerList.push_back(&p);
    }
    kdtree = buildTree(photonPointerList, 0);
}


int findLongestAxis(const std::vector<Photon*> &photonList){
    float xmin = INFINITY, ymin = INFINITY, zmin = INFINITY;
    float xmax = -INFINITY, ymax = -INFINITY, zmax = -INFINITY;
    for (auto p:photonList) {
        xmin = fmin(xmin, p->position.x);
        ymin = fmin(ymin, p->position.y);
        zmin = fmin(zmin, p->position.z);

        xmax = fmax(xmax, p->position.x);
        ymax = fmax(ymax, p->position.y);
        zmax = fmax(zmax, p->position.z);
    }
    float dx = xmax - xmin;
    float dy = ymax - ymin;
    float dz = zmax - zmin;

    float maximum = -INFINITY;
    int index = 0;

    if(dx > maximum){
        index = 0;
        maximum = dx;
    }
    if(dy > maximum){
        index = 1;
        maximum = dy;
    }
    if(dz > maximum){
        index = 2;
        maximum = dz;
    }
//    std::cout << "returned " << index << std::endl;
    return index;
}

PNode * PhotonMap::buildTree(std::vector<Photon*> photonList, int depth){
    if(photonList.size() < 1){
        return NULL;
    }
    int axis = findLongestAxis(photonList);
    std::sort(photonList.begin(), photonList.end(), [&](const Photon *left, const Photon *right){
        return left->position[axis] < right->position[axis];
    });
    int medianIndex = (int)photonList.size() / 2;
    std::vector<Photon*> leftList = std::vector<Photon*>(photonList.begin(), photonList.begin()+medianIndex);
    std::vector<Photon*> rightList = std::vector<Photon*>(photonList.begin()+medianIndex+1, photonList.end());

    photonList[medianIndex]->plane = axis;
    return new PNode(
                    photonList[medianIndex],
                    buildTree(leftList, depth+1),
                    buildTree(rightList, depth+1)

    );
}




std::priority_queue<Result> PhotonMap::kNN(const Pos query, const int k){
    int touched_nodes = 0;
    // Max-heap, sorted by distance.
    std::priority_queue<Result> heap = std::priority_queue<Result>();
    // dummy result seed.
    Result dummy = Result(new Photon(1337, 1337, 1337), INFINITY);
    heap.push(dummy);
    nearest(kdtree, query, touched_nodes, k, heap);
    return heap;
};


float dist(const Photon* a, const Pos &b){
    float dx = a->position.x - b.x;
    float dy = a->position.y - b.y;
    float dz = a->position.z - b.z;
    return dx*dx + dy*dy + dz*dz;
}

void PhotonMap::nearest(const PNode *root, const Pos &query, int &visited, const int k, std::priority_queue<Result> &heap){
    double d, dx, dx2;
    if (!root) { return; }
    int axis = root->photon->plane;
    d = dist(root->photon, query);
    dx = root->photon->position[axis] - query[axis];
    dx2 = dx * dx;

    visited ++;
    if (d < heap.top().dx) { // worst distance from query currently in prio queue
        // Insert this, kicking out the lowest one if neccessary.
        Result r = Result(root->photon, d);
        heap.push(r);
        if(heap.size() > k){ heap.pop(); }
    }
    nearest(dx > 0 ? root->left : root->right, query, visited, k, heap);
    if (dx2 >= heap.top().dx) { return; }  // worst distance again
    nearest(dx > 0 ? root->right : root->left, query, visited, k, heap);
}
