//
//  Point.h
//  reyes
//
//  Created by Arve Nygård on 14/01/15.
//  Copyright (c) 2015 Arve Nygård. All rights reserved.
//

#ifndef __reyes__Point3f__
#define __reyes__Point3f__
#include <iostream>
#include <stdio.h>
#include "Vec3f.h"
class Vec3f;
class Point {
public:
    float x;
    float y;
    float z;
    float w;
    float u;
    float v;
    Vec3f N, Ng;
    Vec3f dPdu;
    Vec3f dPdv;
    Color Cs; // Surface color. RGB, [0,1] -> [black,white]
    Color Os; // Surface Opacity RGB [0,1] -> [transparent, opaque]
    Color Ci; // Apparent color after shading
    Color Oi; // Apparent opacity after shading
    
    Point();
    Point(const float x, const float y, const float z, const float w);
    Point(const Vec3f& vector);
    void perspectiveDivide();
    
    static float distance(const Point& a, const Point&b);
    static float distanceXY(const Point& a, const Point&b);
    Point operator+(const Point other);
    Point operator-(const Point other)const;
    
    friend std::ostream& operator<<(std::ostream& os, const Point& dt);
};

#endif /* defined(__reyes__Point3f__) */
