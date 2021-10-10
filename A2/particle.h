#ifndef PARTICLE_H
#define PARTICLE_H

#include "../A1/mathLib2D.h"


struct Colour {
    float r;
    float g;
    float b;
};

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

