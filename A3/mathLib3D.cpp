#include "headers/mathLib3D.h"
#include <cmath>

Point3D::Point3D(){}

Point3D::Point3D(float inX, float inY, float inZ){
    mX = inX;
    mY = inY;
    mZ = inZ;
}

float Point3D::distanceTo(Point3D other){
    return sqrt(pow(mX-other.mX,2)+pow(mY-other.mY,2)+pow(mZ-other.mZ,2));
}

float Point3D::fastDistanceTo(Point3D other){
    return pow(mX-other.mX,2)+pow(mY-other.mY,2)+pow(mZ-other.mZ,2);
}

Vec3D::Vec3D(){}

Vec3D::Vec3D(float inX, float inY, float inZ){
    mX = inX;
    mY = inY;
    mZ = inZ;
}

float Vec3D::length(){
    return sqrt(pow(mX,2)+pow(mY,2)+pow(mZ,2));
}

Vec3D Vec3D::normalize(){
    float l = length();
    return Vec3D(mX/l,mY/l,mZ/l);
}

Vec3D Vec3D::multiply(float scalar){
    return Vec3D(mX*scalar,mY*scalar,mZ*scalar);
}

Point3D Vec3D::movePoint(Point3D source){
    return Point3D(source.mX+mX,source.mY+mY,source.mZ+mZ);
}

Vec3D Vec3D::createVector(Point3D p1, Point3D p2){
    return Vec3D(p2.mX=p1.mX,p2.mY-p1.mY,p2.mZ-p1.mZ);
}