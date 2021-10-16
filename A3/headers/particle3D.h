#ifndef PARTICLE_3D
#define PARTICLE_3D

#include "mathLib3D.h"
#include "../../A2/colour.h"

enum Material {Metal, Wood, Plastic};


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

    void update();
    void markForDeletion();
    bool shouldDelete();

    constexpr static float TERMINAL_VELOCITY = 100;
    constexpr static float MIN_SPEED = 0.005f;
    const static int PARTICLE_LIFESPAN = 75;



private:
    bool marked_for_deletion;
};

#endif // PARTICLE_3D
