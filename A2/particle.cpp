#include "particle.h"

Particle::Particle(){}


Particle::Particle(Point2D position, Colour colour, int size, Vec2D direction, float range, float speed){
    this->position = position;
    this->colour = colour;
    this->size = size;
    this->direction = direction;
    this->range = range;
    this->speed = speed;
}




