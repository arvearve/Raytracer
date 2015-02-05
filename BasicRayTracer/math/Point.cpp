//
//  Point3f.cpp
//  reyes
//
//  Created by Arve Nygård on 14/01/15.
//  Copyright (c) 2015 Arve Nygård. All rights reserved.
//

#include "Point.h"

Point::Point():x(0), y(0), z(0), w(1){
    u=v=0;
    Ng = Vec3f(1, 1, 1);
    Cs = Color(0, 0, 0);
    Os = Color(1,1,1);
}

Point::Point(const float x, const float y, const float z, const float w):x(x), y(y), z(z), w(w){
    u=0;
    v=0;
    Cs = Color(0,0,0);
    Os = Color(1,1,1);
}

//Point::Point(Point &p):x(p.x), y(p.y), z(p.z), u(p.u), v(p.v), color(p.color), normal(p.normal){}

Point::Point(const Vec3f& vector){
    x = vector.x;
    y = vector.y;
    z = vector.z;
    w = 1;
    u=0;
    v = 0;
    Cs = Vec3f(0,0,0);
    Os = Color(1,1,1);
}

float Point::distance(const Point& a, const Point&b){
    return sqrt(
                (a.x-b.x) * (a.x-b.x) +
                (a.y-b.y) * (a.y-b.y) +
                (a.z-b.z) * (a.z-b.z));
}
float Point::distanceXY(const Point& a, const Point&b){
    return sqrt(
                (a.x-b.x) * (a.x-b.x) +
                (a.y-b.y) * (a.y-b.y));
}

void Point::perspectiveDivide(){
    x = x/w;
    y = y/w;
    z = z/w;
    w = w/w;
}

Point Point::operator+(const Point other){
    return Point(x+other.x, y+other.y, z+other.z, w + other.w);
}

Point Point::operator-(const Point other) const{
    return Point(x-other.x, y-other.y, z-other.z, w-other.w);
}

std::ostream& operator<<(std::ostream& os, const Point& p){
    // os << "[" << p.x << "," << p.y << "," << p.z << "," << p.w << "]" << std::endl;
    os << "[";
    os << p.x << "," << p.y << ", " << p.z;
    os << "]";
    return os;
}