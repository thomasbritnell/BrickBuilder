#ifndef PARTICLE_LIST_H
#define PARTICLE_LIST_H

#include "particle3D.h"
#include <vector>

/**
 * @brief List of Particle3D with added functionality for updating, deleting old particles, etc. 
 * 
 */
class ParticleList{
public:
    
    ParticleList(std::vector<Particle3D> particles, Point3D origin, Vec3D direciton);
    ParticleList(std::vector<Particle3D> particles, Point3D origin);
    ParticleList(std::vector<Particle3D> particles);
    ParticleList();
    /**
     * @brief Creates a new list of particles that have not been marked for deletion.
     * 
     */
    void removeDeleted();
    /**
     * @brief Updates the state variable of particle in the list
     * 
     * @param friction friction mode on or off
     */
    void updateParticles(bool friction);
    //The amount of particles that come out of the fountain every frame
    const static int BURST_SIZE = 2;
    Point3D origin; //where the particles originate from 
    Vec3D direction; //the direction of the particle system

    void reset();//delete the particle list

    void addParticle(Particle3D particle);//add a particle to the list

    std::vector<Particle3D> particles;

    bool trailOn = false;//Whether the particle trail is active or not for the system 
};


#endif