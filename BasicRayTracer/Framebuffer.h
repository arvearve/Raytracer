//
//  framebuffer.h
//  reyes
//
//  Created by Arve Nygård on 18/01/15.
//  Copyright (c) 2015 Arve Nygård. All rights reserved.
//

#ifndef __reyes__framebuffer__
#define __reyes__framebuffer__

#include <stdio.h>
#include <math.h>
#include <sstream>
#include <vector>
#include "Vec3f.h"
#include "Ray.h"
#include "scene_io.h"
#include "EasyBMP.h"

/* Generate a random value between -distance and distance.
 Used to jitter sample points inside their subpixel.
 */

struct Sample {
    Sample(const Pos p):pos(p), color(Colr(0,0,0)){}
    Pos pos;
    Colr color;
};

struct Pixel {
    float jitter(const float distance) const;
    int numSamples;
    std::vector<Sample> samples;
    Colr filteredColor;

    Pixel(int numSamples):numSamples(numSamples), samples(std::vector<Sample>()), filteredColor(Colr(0,0,0)){};
    void filter();
};

class Framebuffer {
private:
    std::vector<Pixel> pixels;
    int WIDTH;
    int HEIGHT;
    int samples;

    int round(double d);
    void initblack();
public:
    Framebuffer(const int w, const int h, const int samples):WIDTH(w), HEIGHT(h), samples(sqrt(samples)){
        init();
    };
    void init();
    float jitter(const float distance) const;
    void setPixelSamples(const float x);
    void render(char *filename);
    void filter();
    void saveFile(char *filename);
};
#endif /* defined(__reyes__framebuffer__) */
