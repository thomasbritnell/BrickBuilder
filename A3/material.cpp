#include "headers/material.h"


Material::Material(MaterialType type){

    this->type = type;

    switch (type){
        case MaterialType::plastic :
            this->amb[0] = 0.0f;
            this->amb[1] = 0.1f;
            this->amb[2] = 0.06f ;
            this->amb[3] = 1.0f;

            this->diff[0] = 0.0f;
            this->diff[1] = 0.50980392f;
            this->diff[2] = 0.50980392f;
            this->diff[3] = 1.0f;

            this->spec[0] = 0.50196078f;
            this->spec[1] = 0.50196078f;
            this->spec[2] = 0.50196078f;
            this->spec[3] = 1.0f;


            this->shiny[0] = 32.0f ;

        break;
        case MaterialType::ruby :

            this->amb[0] =0.1745f ;
            this->amb[1] =  0.01175f;
            this->amb[2] =  0.01175f;
            this->amb[3] =  0.55f;

            this->diff[0] = 0.61424f;
            this->diff[1] =  0.04136f;
            this->diff[2] =  0.04136f;
            this->diff[3] = 0.55f;

            this->spec[0] = 0.727811f;
            this->spec[1] = 0.626959f;
            this->spec[2] =  0.626959f; 
            this->spec[3] = 0.55f;


            this->shiny[0] = 76.8f ;
        break;
        case MaterialType::gold :

            this->amb[0] = 0.24725f;
            this->amb[1] = 0.1995f;
            this->amb[2] = 0.0745f;
            this->amb[3] = 1.0f;

            this->diff[0] = 0.75164f;
            this->diff[1] = 0.60648f;
            this->diff[2] = 0.22648f;
            this->diff[3] = 1.0f;

            this->spec[0] = 0.628281f;
            this->spec[1] = 0.555802f;
            this->spec[2] = 0.366065f;
            this->spec[3] = 1.0f;

            this->shiny[0] = 51.2f;
        break;
    }
}

Material::Material():Material(MaterialType::plastic){}
