#ifndef MATERIAL_H
#define MATERIAL_H

/**
 * Ruby = 0 Gold = 1 Plastic = 2
 * 
 */
enum MaterialType{
    ruby,gold,plastic
};

/**
 * Holds the material properties for glMaterialfv for a given material.
 * 
 */
class Material{
public:
    /**
     * Default material is plastic
     * 
     */
    Material();
    Material(MaterialType type);
    MaterialType type;
    float amb[4]; //ambient colour
    float diff[4]; //diffuse light
    float spec[4]; //specular light
    float shiny[1]; //shiny highlight colour
};



#endif