//
//  Matrix.cpp
//  reyes
//
//  Created by Arve Nygård on 13/01/15.
//  Copyright (c) 2015 Arve Nygård. All rights reserved.
//

#include "Mat4f.h"

Mat4f::Mat4f() {
    // Identity matrix
    for(int i = 0; i < 4; i++){
        val[i][i] = 1.0f;
    }
}

Mat4f::Mat4f(float a11, float a12, float a13, float a14,
             float a21, float a22, float a23, float a24,
             float a31, float a32, float a33, float a34,
             float a41, float a42, float a43, float a44) {
    // Matrix literal
    val[0][0] = a11; val[0][1] = a12, val[0][2] = a13, val[0][3] = a14;
    val[1][0] = a21; val[1][1] = a22, val[1][2] = a23, val[1][3] = a24;
    val[2][0] = a31; val[2][1] = a32, val[2][2] = a33, val[2][3] = a34;
    val[3][0] = a41; val[3][1] = a42, val[3][2] = a43, val[3][3] = a44;
}

Mat4f Mat4f::translate(const float dx, const float dy, const float dz) {
    return Mat4f(1.0f, 0.0f, 0.0f, dx,
                 0.0f, 1.0f, 0.0f, dy,
                 0.0f, 0.0f, 1.0f, dz,
                 0.0f, 0.0f, 0.0f, 1.0f);
}

Mat4f Mat4f::scale(const float sx, const float sy, const float sz){
    return Mat4f( sx,  0.0f, 0.0f, 0.0f,
                 0.0f,  sy,  0.0f, 0.0f,
                 0.0f, 0.0f,  sz,  0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f);
}

Mat4f Mat4f::rotX(float angle){
	return Mat4f(1, 0.0, 0.0, 0.0,
		0.0, cos(angle), -sin(angle), 0.0,
		0.0, sin(angle), cos(angle), 0.0,
		0.0, 0.0, 0.0, 1);
}

Mat4f Mat4f::rotY(float angle){
	return Mat4f(cos(angle), 0.0, -sin(angle), 0.0,
		0, 1, 0.0, 0.0,
		sin(angle), 0, cos(angle), 0.0,
		0.0, 0.0, 0.0, 1);
}

Mat4f Mat4f::rotZ(float angle){
	return Mat4f(cos(angle), -sin(angle), 0.0, 0.0,
		sin(angle), cos(angle), 0.0, 0.0,
		0.0, 0.0, 1, 0.0,
		0.0, 0.0, 0.0, 1);
}



Mat4f Mat4f::rotate(const float angle, const float dx, const float dy, const float dz){
    // RI speaks about angles in degrees. C trig functions accept radians as arguments. Radian = degree * (PI/180)
    float theta = angle * (M_PI / 180);

    /*  http://paulbourke.net/geometry/rotate/
     1. Rotate about x to align with xz plane
     2. Rotate about y to align with x axis
     3. Rotate <angle> about z
     4. Rotate back around y
     5. Rotate back around x */

    Vec3f u = Vec3f::normalize(Vec3f(dx, dy, dz)); // unit vector describing rotation axis.
    float a = u.x;
    float b = u.y;
    float c = u.z;
    float d = sqrt( b*b + c*c);
	Mat4f rotx = Mat4f(1.0, 0.0, 0.0, 0.0,
		0.0, c / d, -b / d, 0.0,
		0.0, b / d, c / d, 0.0,
		0.0, 0.0, 0.0, 1.0);
    
	Mat4f rotx_i = Mat4f(1.0, 0.0, 0.0, 0.0,
		0.0, c / d, b / d, 0.0,
		0.0, -b / d, c / d, 0.0,
		0.0, 0.0, 0.0, 1.0);
    
	Mat4f roty = Mat4f(d, 0.0, -a, 0.0,
		0.0, 1, 0.0, 0.0,
		a, 0.0, d, 0.0,
		0.0, 0.0, 0.0, 1);
    
	Mat4f roty_i = Mat4f(d, 0.0, a, 0.0,
		0.0, 1.0, 0.0, 0.0,
		-a, 0.0, d, 0.0,
		0.0, 0.0, 0.0, 1.0);
    Mat4f rotz = rotZ(theta);
    Mat4f result = roty_i * rotz * roty;
    if(d != 0){ result = rotx_i * result * rotx;}
    return result;
}

Mat4f Mat4f::perspective(const float fov, const float ratio, const float hither, const float yon){
    float fov_rads = fov * M_PI / 180.0f;
    float X = 1.0f/std::tan(fov_rads/2.0f);
    float Y = 1.0f/std::tan(fov_rads/2.0f*ratio);
    float A = yon / (yon-hither);
    float B = (- yon - hither) / (yon-hither);
    return Mat4f( X  , 0.0f, 0.0f, 0.0f,
                 0.0f,  Y  , 0.0f, 0.0f,
                 0.0f, 0.0f,  A  , 0.0f,
                 0.0f, 0.0f, 1.0f,  B  );
}


Mat4f Mat4f::orthographic(float xmin, float xmax, float ymin, float ymax, float hither, float yon){
    // http://en.wikipedia.org/wiki/Orthographic_projection
    Mat4f result = Mat4f();

    result(0,0) = 2.0f / (xmax - xmin);
    result(1,1) = 2.0f / (ymax - ymin);
    result(2,2) = -2.0f / (yon - hither);
    result(3,3) = 1.0f;
    result(0,3) = -(xmax + xmin) / (xmax - xmin);
    result(1,3) = -(ymax + ymin) / (ymax - ymin);
    result(2,3) = (yon + hither) / (yon - hither);

    return result;
}




// Matrix * matrix
Mat4f operator*(const Mat4f &a, const Mat4f &b){
    Mat4f result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result(i,j) = a(i,0) * b(0,j) +
                          a(i,1) * b(1,j) +
                          a(i,2) * b(2,j) +
                          a(i,3) * b(3,j);
        }
    }
    return result;
}
// Matrix * vector
Vec3f operator*(const Mat4f &a, const Vec3f &b){
    return Vec3f(
                 a(0, 0)*b.x + a(0,1)*b.y + a(0,2)*b.z + a(0,3),
                 a(1, 0)*b.x + a(1,1)*b.y + a(1,2)*b.z + a(1,3),
                 a(2, 0)*b.x + a(2,1)*b.y + a(2,2)*b.z + a(2,3)
                 );
}

//// Matrix * point
//Point operator*(const Mat4f &a, const Point &b){
//    Point result = Point(b);
//    result.x = a(0, 0)*b.x + a(0,1)*b.y + a(0,2)*b.z + a(0,3)*b.w;
//    result.y = a(1, 0)*b.x + a(1,1)*b.y + a(1,2)*b.z + a(1,3)*b.w;
//    result.z = a(2, 0)*b.x + a(2,1)*b.y + a(2,2)*b.z + a(2,3)*b.w;
//    result.w = a(3, 0)*b.x + a(3,1)*b.y + a(3,2)*b.z + a(3,3)*b.w;
//   return result;
//}

std::ostream& operator<<(std::ostream& os, const Mat4f& m){
    os << "[" << m(0,0) << " " << m(0,1) << " " << m(0,2) << " " << m(0,3) << "]" << std::endl;
    os << "[" << m(1,0) << " " << m(1,1) << " " << m(1,2) << " " << m(1,3) << "]" << std::endl;
    os << "[" << m(2,0) << " " << m(2,1) << " " << m(2,2) << " " << m(2,3) << "]" << std::endl;
    os << "[" << m(3,0) << " " << m(3,1) << " " << m(3,2) << " " << m(3,3) << "]" << std::endl;
    return os;
}

Mat4f Mat4f::inverse(const Mat4f &m){
    Mat4f result = Mat4f();
    result(0,0) = m(1,2)*m(2,3)*m(3,1) - m(1,3)*m(2,2)*m(3,1) + m(1,3)*m(2,1)*m(3,2) - m(1,1)*m(2,3)*m(3,2) - m(1,2)*m(2,1)*m(3,3) + m(1,1)*m(2,2)*m(3,3);
    result(0,1) = m(0,3)*m(2,2)*m(3,1) - m(0,2)*m(2,3)*m(3,1) - m(0,3)*m(2,1)*m(3,2) + m(0,1)*m(2,3)*m(3,2) + m(0,2)*m(2,1)*m(3,3) - m(0,1)*m(2,2)*m(3,3);
    result(0,2) = m(0,2)*m(1,3)*m(3,1) - m(0,3)*m(1,2)*m(3,1) + m(0,3)*m(1,1)*m(3,2) - m(0,1)*m(1,3)*m(3,2) - m(0,2)*m(1,1)*m(3,3) + m(0,1)*m(1,2)*m(3,3);
    result(0,3) = m(0,3)*m(1,2)*m(2,1) - m(0,2)*m(1,3)*m(2,1) - m(0,3)*m(1,1)*m(2,2) + m(0,1)*m(1,3)*m(2,2) + m(0,2)*m(1,1)*m(2,3) - m(0,1)*m(1,2)*m(2,3);
    result(1,0) = m(1,3)*m(2,2)*m(3,0) - m(1,2)*m(2,3)*m(3,0) - m(1,3)*m(2,0)*m(3,2) + m(1,0)*m(2,3)*m(3,2) + m(1,2)*m(2,0)*m(3,3) - m(1,0)*m(2,2)*m(3,3);
    result(1,1) = m(0,2)*m(2,3)*m(3,0) - m(0,3)*m(2,2)*m(3,0) + m(0,3)*m(2,0)*m(3,2) - m(0,0)*m(2,3)*m(3,2) - m(0,2)*m(2,0)*m(3,3) + m(0,0)*m(2,2)*m(3,3);
    result(1,2) = m(0,3)*m(1,2)*m(3,0) - m(0,2)*m(1,3)*m(3,0) - m(0,3)*m(1,0)*m(3,2) + m(0,0)*m(1,3)*m(3,2) + m(0,2)*m(1,0)*m(3,3) - m(0,0)*m(1,2)*m(3,3);
    result(1,3) = m(0,2)*m(1,3)*m(2,0) - m(0,3)*m(1,2)*m(2,0) + m(0,3)*m(1,0)*m(2,2) - m(0,0)*m(1,3)*m(2,2) - m(0,2)*m(1,0)*m(2,3) + m(0,0)*m(1,2)*m(2,3);
    result(2,0) = m(1,1)*m(2,3)*m(3,0) - m(1,3)*m(2,1)*m(3,0) + m(1,3)*m(2,0)*m(3,1) - m(1,0)*m(2,3)*m(3,1) - m(1,1)*m(2,0)*m(3,3) + m(1,0)*m(2,1)*m(3,3);
    result(2,1) = m(0,3)*m(2,1)*m(3,0) - m(0,1)*m(2,3)*m(3,0) - m(0,3)*m(2,0)*m(3,1) + m(0,0)*m(2,3)*m(3,1) + m(0,1)*m(2,0)*m(3,3) - m(0,0)*m(2,1)*m(3,3);
    result(2,2) = m(0,1)*m(1,3)*m(3,0) - m(0,3)*m(1,1)*m(3,0) + m(0,3)*m(1,0)*m(3,1) - m(0,0)*m(1,3)*m(3,1) - m(0,1)*m(1,0)*m(3,3) + m(0,0)*m(1,1)*m(3,3);
    result(2,3) = m(0,3)*m(1,1)*m(2,0) - m(0,1)*m(1,3)*m(2,0) - m(0,3)*m(1,0)*m(2,1) + m(0,0)*m(1,3)*m(2,1) + m(0,1)*m(1,0)*m(2,3) - m(0,0)*m(1,1)*m(2,3);
    result(3,0) = m(1,2)*m(2,1)*m(3,0) - m(1,1)*m(2,2)*m(3,0) - m(1,2)*m(2,0)*m(3,1) + m(1,0)*m(2,2)*m(3,1) + m(1,1)*m(2,0)*m(3,2) - m(1,0)*m(2,1)*m(3,2);
    result(3,1) = m(0,1)*m(2,2)*m(3,0) - m(0,2)*m(2,1)*m(3,0) + m(0,2)*m(2,0)*m(3,1) - m(0,0)*m(2,2)*m(3,1) - m(0,1)*m(2,0)*m(3,2) + m(0,0)*m(2,1)*m(3,2);
    result(3,2) = m(0,2)*m(1,1)*m(3,0) - m(0,1)*m(1,2)*m(3,0) - m(0,2)*m(1,0)*m(3,1) + m(0,0)*m(1,2)*m(3,1) + m(0,1)*m(1,0)*m(3,2) - m(0,0)*m(1,1)*m(3,2);
    result(3,3) = m(0,1)*m(1,2)*m(2,0) - m(0,2)*m(1,1)*m(2,0) + m(0,2)*m(1,0)*m(2,1) - m(0,0)*m(1,2)*m(2,1) - m(0,1)*m(1,0)*m(2,2) + m(0,0)*m(1,1)*m(2,2);
    float det = Mat4f::determinant(m);

    return  result * (1.0f/det);
}

Mat4f Mat4f::transpose(const Mat4f &m){
    return Mat4f(
                 m(0,0),m(1,0),m(2,0),m(3,0),
                 m(0,1),m(1,1),m(2,1),m(3,1),
                 m(0,2),m(1,2),m(2,2),m(3,2),
                 m(0,3),m(1,3),m(2,3),m(3,3)
    );
}

float Mat4f::determinant(const Mat4f &m) {
    return
    m(0,3)*m(1,2)*m(2,1)*m(3,0) - m(0,2)*m(1,3)*m(2,1)*m(3,0) - m(0,3)*m(1,1)*m(2,2)*m(3,0) + m(0,1)*m(1,3)*m(2,2)*m(3,0)+
    m(0,2)*m(1,1)*m(2,3)*m(3,0) - m(0,1)*m(1,2)*m(2,3)*m(3,0) - m(0,3)*m(1,2)*m(2,0)*m(3,1) + m(0,2)*m(1,3)*m(2,0)*m(3,1)+
    m(0,3)*m(1,0)*m(2,2)*m(3,1) - m(0,0)*m(1,3)*m(2,2)*m(3,1) - m(0,2)*m(1,0)*m(2,3)*m(3,1) + m(0,0)*m(1,2)*m(2,3)*m(3,1)+
    m(0,3)*m(1,1)*m(2,0)*m(3,2) - m(0,1)*m(1,3)*m(2,0)*m(3,2) - m(0,3)*m(1,0)*m(2,1)*m(3,2) + m(0,0)*m(1,3)*m(2,1)*m(3,2)+
    m(0,1)*m(1,0)*m(2,3)*m(3,2) - m(0,0)*m(1,1)*m(2,3)*m(3,2) - m(0,2)*m(1,1)*m(2,0)*m(3,3) + m(0,1)*m(1,2)*m(2,0)*m(3,3)+
    m(0,2)*m(1,0)*m(2,1)*m(3,3) - m(0,0)*m(1,2)*m(2,1)*m(3,3) - m(0,1)*m(1,0)*m(2,2)*m(3,3) + m(0,0)*m(1,1)*m(2,2)*m(3,3);

}