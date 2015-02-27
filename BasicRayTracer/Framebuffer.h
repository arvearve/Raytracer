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


struct Pixel {

    Pos position;
    int numSamples;
    std::vector<Colr> samples;
    Colr filteredColor;

    Pixel(int numSamples, Pos position):position(position),numSamples(numSamples), samples(std::vector<Colr>()), filteredColor(Colr(0,0,0)){};
    void filter();
    float jitter(const float distance) const;
};

class Framebuffer {
private:
    std::vector<Pixel> pixels;
    int WIDTH;
    int HEIGHT;
    int samples;

    void initblack();
public:
    Framebuffer(const int w, const int h, const int samples):WIDTH(w), HEIGHT(h), samples(sqrt(samples)){};
    void init(const float focaldistance, const float focalDistance);
    void  initPinhole(const float sensorDistance);
    void renderLens(char* filename, const float sensorDistance);
    void renderPinhole(char *filename, float sensorDistance);

    float jitter(const float distance) const;

    void filter();
    void saveFile(char *filename, bool flip);
};
#endif /* defined(__reyes__framebuffer__) */
