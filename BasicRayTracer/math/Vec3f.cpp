//
//  Vec3f.cpp
//  reyes
//
//  Created by Arve Nygård on 13/01/15.
//  Copyright (c) 2015 Arve Nygård. All rights reserved.
//

#include "Vec3f.h"

Vec3f & Vec3f::normalizeColor(){
    float div = fmaxf(x, fmaxf(y, z));
    x = x/div;
    y = y/div;
    z = z/div;
    return *this;
}

RGBApixel Vec3f::toRGBAPixel() const{
    RGBApixel pixel;
    pixel.Red = (int)x;
    pixel.Blue = (int)y;
    pixel.Green = (int)z;
    pixel.Alpha = 255;
    return pixel;
}

float* Vec3f::toArray() const {
    return new float[3]{x, y, z};
}

Colr Vec3f::capColor(){
    x = fminf(x, 1.0);
    y = fminf(y, 1.0);
    z = fminf(z, 1.0);
    return *this;
}


std::ostream& operator<<(std::ostream &strm, const Vec3f &vec) {
    return strm << "[" << vec.x  << vec.y << vec.z << "]";
}