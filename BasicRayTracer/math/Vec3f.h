
//
//  Vec3f.h
//  reyes
//
//  Created by Arve Nygård on 13/01/15.
//  Copyright (c) 2015 Arve Nygård. All rights reserved.
//

#ifndef __reyes__Vec3f__
#define __reyes__Vec3f__
#include <iostream>
#include <cmath>
#include <stdio.h>
#include "EasyBMP.h"

class Vec3f {
public:
    float x, y, z;
    Vec3f():x(0),y(0),z(0){}
    Vec3f(float x, float y, float z): x(x), y(y),z(z){}
    Vec3f(const float color[3]):x(color[0]), y(color[1]), z(color[2]){}
    float length(void) const;
    Vec3f& normalize();
    RGBApixel toRGBAPixel() const;
    void capColor();
    static float dot(const Vec3f &a, const Vec3f &b);
    static Vec3f cross(const Vec3f &a, const Vec3f &b);
    static float angle(const Vec3f &a, const Vec3f &b);
    static Vec3f normalize(const Vec3f &v);

    Vec3f operator+(const Vec3f &other) const;
    Vec3f operator-(const Vec3f &other) const;
    Vec3f operator*(const Vec3f &other);
    Vec3f operator*(const float scale) const;
    friend std::ostream& operator<<(std::ostream&, const Vec3f &vec);
    
};
typedef Vec3f Colr;
typedef Vec3f Pos;
#endif /* defined(__reyes__Vec3f__) */
