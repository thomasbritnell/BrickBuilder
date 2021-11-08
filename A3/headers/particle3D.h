#ifndef PARTICLE_3D
#define PARTICLE_3D

#include "mathLib3D.h"
#include "colour.h"
#include "material.h"
#include <vector>

/**
 * Effectively a wrapper for the Point3D class which adds motion information (including direction, rotation)
 *  as well as more properties for displaying it (colour, size, material, age)
 *
 */
class Particle3D
{
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

    /**
     * @brief Update the particle's state variables
     * 
     * @param friction whether friction mode is active or not
     */
    void update(bool friction);
    /**
     * @brief Particle marked for deletion will be removed next frame.
     * 
     */
    void markForDeletion();
    /**
     * @brief Checks to see if the particle has been marked and will be deleted soon. 
     */
    bool shouldDelete();
    /**
     * @brief Add current position to the trail
     * 
     */
    void addToTrail();
    /**
     * @brief Remove the trail of a particle. For when it dies.
     * 
     */
    void clearTrail();

    const static int PARTICLE_LIFESPAN = 250;

private:
    bool marked_for_deletion;
};

#endif // PARTICLE_3D
