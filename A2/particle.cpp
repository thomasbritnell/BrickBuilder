#include "particle.h"
#include <iostream>

Particle::Particle(){}


Particle::Particle(Point2D position, Colour colour, int size, Vec2D direction, float range, float speed){
    this->position = position;
    this->colour = colour;
    this->size = size;
    this->direction = direction;
    this->range = range;
    this->speed = speed;
    this->moving_to_target = false;
    this->target = Point2D(0,0);
}


bool Particle::collideWith(Particle other){
    return (this->position.distanceTo(other.position) <= (this->size + other.size)/2);
}



