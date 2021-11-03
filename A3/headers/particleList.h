#ifndef PARTICLE_LIST_H
#define PARTICLE_LIST_H

#include "particle3D.h"
#include <vector>

class ParticleList{
public:
    
    ParticleList(std::vector<Particle3D> particles, Point3D origin, Vec3D direciton);
    ParticleList(std::vector<Particle3D> particles);
    ParticleList();
    void removeDeleted();
    void updateParticles();
    const static int BURST_SIZE = 2;
    Point3D origin;
    Vec3D direction;

    void addParticle(Particle3D particle);

    std::vector<Particle3D> particles;
};


#endif