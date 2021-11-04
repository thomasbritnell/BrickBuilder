#include "headers/particleList.h"
#include <random>

ParticleList::ParticleList(std::vector<Particle3D> particles, Point3D origin, Vec3D direction){
    this->particles = particles;
    this->origin = origin;
    this->direction = direction;
}

ParticleList::ParticleList(std::vector<Particle3D> particles, Point3D origin) : ParticleList(particles, origin, Vec3D(0.0005f,1,0.0005f)){}

ParticleList::ParticleList(std::vector<Particle3D> particles) : ParticleList(particles, Point3D(0,0,0), Vec3D(0.0005f,1,0.0005f)){}

ParticleList::ParticleList() : ParticleList(std::vector<Particle3D>(), Point3D(0,0,0), Vec3D(0.0005f,1,0.0005f)){} //if this is a problem then yeah its probaly hthis line

void ParticleList::removeDeleted(){

    std::vector<Particle3D> survivors;


    // for(auto p = this->particles.end(); p != this->particles.begin(); --p){
        
    // }
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

void ParticleList::reset(){
    this->particles.clear();
}

void ParticleList::addParticle(Particle3D particle){
    this->particles.push_back(particle);
}