//
//  Matrix.h
//  reyes
//
//  Created by Arve Nygård on 13/01/15.
//  Copyright (c) 2015 Arve Nygård. All rights reserved.
//

#ifndef __reyes__Matrix__
#define __reyes__Matrix__
#define _USE_MATH_DEFINES
#include <cmath>
#include <stdio.h>
#include "Vec3f.h"
//#include "Point.h"
class Mat4f {
    float val[4][4] = {};
    static Mat4f rotX(float angle);
    static Mat4f rotY(float angle);
    static Mat4f rotZ(float angle);
public:
    Mat4f();
    Mat4f(  float a11, float a12, float a13, float a14,
          float a21, float a22, float a23, float a24,
          float a31, float a32, float a33, float a34,
          float a41, float a42, float a43, float a44);
    
    static Mat4f translate(const float dx, const float dy, const float dz);
    static Mat4f scale(const float sx, const float sy, const float sz);
    static Mat4f perspective(const float fov, const float ratio, const float hither, const float yon);
    static Mat4f orthographic(float xmin, float xmax, float ymin, float ymax, float hither, float yon);
    static Mat4f rotate(const float angle, const float dx, const float dy, const float dz);
    static Mat4f transpose(const Mat4f &a);
    static Mat4f inverse(const Mat4f &a);
    static float determinant(const Mat4f &a);
    float operator()(int x, int y) const {
        return val[x][y];
    }
    
    float & operator()(int x, int y){
        return val[x][y];
    }

    Mat4f operator*(float scale){
        return Mat4f(
                     val[0][0] * scale,
                     val[0][1] * scale,
                     val[0][2] * scale,
                     val[0][3] * scale,

                     val[1][0] * scale,
                     val[1][1] * scale,
                     val[1][2] * scale,
                     val[1][3] * scale,

                     val[2][0] * scale,
                     val[2][1] * scale,
                     val[2][2] * scale,
                     val[2][3] * scale,

                     val[3][0] * scale,
                     val[3][1] * scale,
                     val[3][2] * scale,
                     val[3][3] * scale
                     );
    }

    friend std::ostream& operator<<(std::ostream& os, const Mat4f& m);
    friend Mat4f operator*(const Mat4f &a, const Mat4f &b);
    friend Vec3f operator*(const Mat4f &a, const Vec3f &b);
//    friend Point operator*(const Mat4f &a, const Point &b);

};


#endif /* defined(__reyes__Matrix__) */
