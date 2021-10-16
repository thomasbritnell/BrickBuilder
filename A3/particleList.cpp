#include "headers/particleList.h"
#include <random>

ParticleList::ParticleList(std::vector<Particle3D> particles, Point3D origin){
    this->particles = particles;
    this->origin = origin;
}

ParticleList::ParticleList(std::vector<Particle3D> particles) : ParticleList(particles, Point3D(0,0,0)){}

ParticleList::ParticleList(){}

void ParticleList::removeDeleted(){

    std::vector<Particle3D> survivors;

    for(Particle3D p : this->particles){
        if (!p.shouldDelete()){
            survivors.push_back(p);
        }
    }
    this->particles = survivors;
}

void ParticleList::updateParticles(){
    removeDeleted();
    
    for(auto p = (&particles)->begin(); p != (&particles)->end(); ++p){
            p->update();
    }
}

void ParticleList::addParticle(Particle3D particle){
    this->particles.push_back(particle);
}