
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

class Vec3f;

typedef Vec3f Colr;
typedef Vec3f Pos;

class Vec3f{
public:
    float x, y, z;
    Vec3f():x(0),y(0),z(0){}
    Vec3f(float x, float y, float z): x(x), y(y),z(z){}
    Vec3f(const float color[3]):x(color[0]), y(color[1]), z(color[2]){}
    inline float length(void) const{
        float lengthSq = x*x + y*y + z*z;
        return std::sqrt(lengthSq);
    };
    inline Vec3f& normalize(){
        float length = this->length();
        x = x / length;
        y = y / length;
        z = z / length;
        return *this;
    };
    Vec3f& normalizeColor();
    RGBApixel toRGBAPixel() const;
    Colr capColor();
    float* toArray() const;
    static inline float dot(const Vec3f &a, const Vec3f &b){
        return  (a.x * b.x
                 + a.y * b.y
                 + a.z * b.z);

    };
    static inline Vec3f cross(const Vec3f &a, const Vec3f &b){
        return Vec3f(a.y*b.z - a.z*b.y,
                     a.z*b.x - a.x*b.z,
                     a.x*b.y - a.y*b.x);
    };

    static inline float angle(const Vec3f &a, const Vec3f &b){
        return acos(dot(a,b) / (a.length() * b.length()));
    };

    static Vec3f normalize(const Vec3f &v){
        float length = v.length();
        Vec3f result = Vec3f(v.x/length, v.y/length, v.z/length);
        return result;
    };

    inline Vec3f operator+(const Vec3f &other) const{
        return Vec3f(x+other.x, y+other.y, z+other.z);
    };
    inline Vec3f operator-(const Vec3f &other) const{
        return Vec3f(x-other.x, y-other.y, z-other.z);
    };

    inline Vec3f operator+=(const Vec3f &other){
        x += other.x, y+= other.y, z += other.z;
        return *this;
    };

    inline Vec3f operator*(const Vec3f &other){
        return Vec3f(x*other.x, y*other.y, z*other.z);
    };

    inline Vec3f operator*(const float scale) const{
        return Vec3f(x*scale, y*scale, z*scale);
    };

    inline float operator[](const int axis){
        if(axis == 0){ return x; }
        if(axis == 1){ return y; }
        else { return z; }
    }

    friend std::ostream& operator<<(std::ostream&, const Vec3f &vec);
    
};
#endif /* defined(__reyes__Vec3f__) */
