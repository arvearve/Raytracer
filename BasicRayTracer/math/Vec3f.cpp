//
//  Vec3f.cpp
//  reyes
//
//  Created by Arve Nygård on 13/01/15.
//  Copyright (c) 2015 Arve Nygård. All rights reserved.
//

#include "Vec3f.h"
Vec3f Vec3f::normalize(const Vec3f &v) {
    float length = v.length();
    Vec3f result = Vec3f(v.x/length, v.y/length, v.z/length);
    return result;
}

Vec3f & Vec3f::normalize(){
    float length = this->length();
	x = x / length;
	y = y / length;
	z = z / length;
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
void Vec3f::capColor(){
    x = fminf(x, 1.0);
    y = fminf(y, 1.0);
    z = fminf(z, 1.0);
}

float Vec3f::length(void) const {
    float lengthSq = x*x + y*y + z*z;
    return std::sqrt(lengthSq);
}

float Vec3f::dot(const Vec3f &a, const Vec3f &b){

    return  (a.x * b.x
             + a.y * b.y
             + a.z * b.z);

}

Vec3f Vec3f::cross(const Vec3f &a, const Vec3f &b){
    return Vec3f(
                 a.y*b.z - a.z*b.y,
                 a.z*b.x - a.x*b.z,
                 a.x*b.y - a.y*b.x);
}

float Vec3f::angle(const Vec3f &a, const Vec3f &b){
    // cos(angle) = dot_product / (a.len * b.len)
    return acos(dot(a,b) / (a.length() * b.length()));
}

Vec3f Vec3f::operator+(const Vec3f &other) const{
    return Vec3f(x+other.x, y+other.y, z+other.z);
}

Vec3f Vec3f::operator-(const Vec3f &other) const{
    return Vec3f(x-other.x, y-other.y, z-other.z);
}

Vec3f Vec3f::operator*(const Vec3f &other){
    return Vec3f(x*other.x, y*other.y, z*other.z);
}

Vec3f Vec3f::operator*(const float scale) const{
    return Vec3f(x*scale, y*scale, z*scale);
};

std::ostream& operator<<(std::ostream &strm, const Vec3f &vec) {
    return strm << "[" << vec.x  << vec.y << vec.z << "]";
}