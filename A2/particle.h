//Thomas Britnell October 2021

#ifndef PARTICLE_H
#define PARTICLE_H

#include "../A1/mathLib2D.h"
#include "colour.h"

class Particle {
public:
    Particle();
    Particle(Point2D position, Colour colour, int size, Vec2D direction, float range, float speed);
    Point2D position;
    Colour colour;
    int size;
    Vec2D direction;
    float range;
    float speed;
    Point2D target;
    bool moving_to_target;
    bool collideWith(Particle other);
};

#endif

