#ifndef PARTICLE_LIST_H
#define PARTICLE_LIST_H

#include "particle3D.h"
#include <vector>

class ParticleList{
public:
    
    ParticleList(std::vector<Particle3D> particles, Point3D origin, Vec3D direciton);
    ParticleList(std::vector<Particle3D> particles, Point3D origin);
    ParticleList(std::vector<Particle3D> particles);
    ParticleList();
    void removeDeleted();
    void updateParticles(bool friction, float* cannon);
    const static int BURST_SIZE = 2;
    Point3D origin;
    Vec3D direction;

    void reset();

    void addParticle(Particle3D particle);

    std::vector<Particle3D> particles;

    bool trailOn = false;

    //std::vector<Particle3D>::iterator selectedParticle;
   // bool selectNewParticle = false;
    // Particle3D* selectedParticle;
};


#endif