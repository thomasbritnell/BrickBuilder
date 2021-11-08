#include "../../A3/headers/mathLib3D.h"
#include "../../A2/colour.h"


#ifndef BRICK_H
#define BRICK_H
class Brick{
    public:
    Brick();
    Brick(int length, int width, int height, Point3D position, float rotation, Colour colour);
    int length; //all are multipliers of n, the scale of the scene
    int width;
    int height;
    Point3D position;
    float rotation;
    Colour colour;
};
#endif

Brick::Brick(){}

Brick::Brick(int length, int width, int height, Point3D position, float rotation, Colour colour){
    this->length = length;
    this->width = width;
    this->height =height;
    this->position = position;
    this->rotation =rotation;
    this->colour = colour;
}

