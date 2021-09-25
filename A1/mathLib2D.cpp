#include "mathLib2D.h"
#include <cmath>




Point2D::Point2D(){}

//Represents a point in 2d space using an x and y coordinate
Point2D::Point2D(float inX, float inY){
    mX = inX;
    mY = inY;
}

//Returns the distance (a float) between 'this' point and the one passed as an argument
float Point2D::distanceTo(Point2D other){
    return sqrt(pow((other.mX-mX),2)+pow((other.mY-mY),2));
}

//Returns an indicator (float) of the relative distance between 'this' point and the one passed as an argument
float Point2D::fastDistanceTo(Point2D other){
    return pow((other.mX-mX),2)+pow((other.mY-mY),2);
}

Vec2D::Vec2D(){}

//A vector in 2d space staring at the origin and extending to (x,y)
Vec2D::Vec2D(float inX, float inY){
    mX = inX;
    mY = inY;
}

//Returns the length (float) of the vector
float Vec2D::length(){
    return sqrt(pow(mX,2) + pow(mY,2));
}

//Returns a copy of the vector with unit length
Vec2D Vec2D::normalize(){
    float v = length();
    return Vec2D(mX/v,mY/v);
}

//Reuturns a copy of the vector with its components multiplied by the scalar value
Vec2D Vec2D::multiply(float scalar){
    return Vec2D(mX*scalar,mY*scalar);
}

//Returns a point resulting from moving the original point along 'this' vector
Point2D Vec2D::movePoint(Point2D source){
    return Point2D(source.mX+mX,source.mY+mY);
}

//Static function that creates a vector from point p1 to point p2
Vec2D Vec2D::createVector(Point2D p1, Point2D p2){
    return Vec2D(p2.mX-p1.mX,p2.mY-p1.mY);
}

