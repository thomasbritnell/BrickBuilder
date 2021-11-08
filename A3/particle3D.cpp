#include "headers/particle3D.h"
#include <cmath>

#include <iostream>
#include <stdlib.h>



Particle3D::Particle3D(Point3D position, Vec3D direction, float speed, Angle rotation, int size, Colour colour, Material material,int age){
    this->position = position;
    this->direction = direction;
    this->speed = speed;
    this->rotation = rotation;
    this->size = size;
    this->colour = colour;
    this->material =material;
    this->age = age;
    this->marked_for_deletion = false;
}

Particle3D::Particle3D() : Particle3D(Point3D(),Vec3D(),0,{0,0,0},1,{1,1,1},Material(),0){};

void Particle3D::markForDeletion(){
    this->marked_for_deletion  =true;
}

bool Particle3D::shouldDelete(){
    return this->marked_for_deletion;
}

//update particle according to speed and direction
void Particle3D::update(bool friction, float* cannon){

    if (this->age < Particle3D::PARTICLE_LIFESPAN){
         //update for gravity
        //std::cout<<age<<std::endl;
        Vec3D acceleration = Vec3D();


        

        if (this->direction.multiply(this->speed).movePoint(this->position).mY >= 0){

        //if above the ground, accelerate due to gravity 
            acceleration = Vec3D(0,-((0.049*size)/speed),0);
        
        }else{

            acceleration = friction ? Vec3D(0,-1.8*this->direction.mY,0) : Vec3D(0,-2*this->direction.mY,0) ;
        }
        

        this->direction = this->direction.add(acceleration);


        this->position = this->direction.multiply(this->speed).movePoint(this->position);
        this->age++;
        this->rotation.rX++;
        this->rotation.rY++;
        this->rotation.rZ++;

    }else{
        markForDeletion();
    }
   // std::cout << "direction * speed " << direction.mY*speed <<std::endl;
}

void Particle3D::addToTrail(){
    this->trail.push_back(this->position);
}

void Particle3D::clearTrail(){
    this->trail.clear();
}