#include "headers/particle3D.h"
#include <cmath>

#include <iostream>
#include <stdlib.h>

Particle3D::Particle3D(){};

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

void Particle3D::markForDeletion(){
    this->marked_for_deletion  =true;
}

bool Particle3D::shouldDelete(){
    return this->marked_for_deletion;
}

//update particle according to speed and direction
void Particle3D::update(){

    if (this->age < Particle3D::PARTICLE_LIFESPAN){
         //update for gravity
        //std::cout<<age<<std::endl;
        if (this->position.mY > -1){
            this->direction = Vec3D(direction.mX,direction.mY-((0.049*size)/speed),direction.mZ);
        
        }else{
            this->direction = Vec3D(direction.mX,0,direction.mZ);
        }
        
        this->position = this->direction.multiply(this->speed).movePoint(this->position);
        this->age++;
    }else{
        markForDeletion();
    }
   // std::cout << "direction * speed " << direction.mY*speed <<std::endl;
}