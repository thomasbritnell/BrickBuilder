#ifndef MATERIAL_H
#define MATERIAL_H

enum MaterialType{
    ruby,gold,plastic
};

class Material{
    public:
        Material();
        Material(MaterialType type);
        MaterialType type;
        float amb[4];
        float diff[4];
        float spec[4];
        float shiny[1];
};

#endif