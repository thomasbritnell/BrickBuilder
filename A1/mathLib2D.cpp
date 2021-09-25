#include "mathLib2D.h"
#include <cmath>




Point2D::Point2D(){}

Point2D::Point2D(float inX, float inY){
    mX = inX;
    mY = inY;
}
    
float Point2D::distanceTo(Point2D other){
    return sqrt(pow((other.mX-mX),2)+pow((other.mY-mY),2));
}

float Point2D::fastDistanceTo(Point2D other){
    return pow((other.mX-mX),2)+pow((other.mY-mY),2);
}

Vec2D::Vec2D(){}

Vec2D::Vec2D(float inX, float inY){
    mX = inX;
    mY = inY;
}

float Vec2D::length(){
    return sqrt(pow(mX,2) + pow(mY,2));
}

Vec2D Vec2D::normalize(){
    float v = length();
    return Vec2D(mX/v,mY/v);
}

Vec2D Vec2D::multiply(float scalar){
    return Vec2D(mX*scalar,mY*scalar);
}

Point2D Vec2D::movePoint(Point2D source){
    return Point2D(source.mX+mX,source.mY+mY);
}

Vec2D Vec2D::createVector(Point2D p1, Point2D p2){
    return Vec2D(p2.mX-p1.mX,p2.mY-p1.mY);
}

