#ifndef PARTICLE_3D
#define PARTICLE_3D

#include "mathLib3D.h"
#include "colour.h"
#include "material.h"
#include <vector>

class Particle3D{
public:
    Particle3D();
    Particle3D(Point3D position, Vec3D direction, float speed, Angle rotation, int size, Colour colour, Material material, int age);
    Point3D position;
    Vec3D direction;
    float speed;
    Angle rotation;
    int size;
    Colour colour;
    Material material;
    int age;
    std::vector<Point3D> trail;

    void update(bool friction, float* cannon);
    void markForDeletion();
    bool shouldDelete();
        void addToTrail();
    void clearTrail();

    constexpr static float TERMINAL_VELOCITY = 100;
    constexpr static float MIN_SPEED = 0.005f;
    const static int PARTICLE_LIFESPAN = 250;



private:
    bool marked_for_deletion;
};

#endif // PARTICLE_3D
