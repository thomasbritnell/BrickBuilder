#ifndef MATERIAL_H
#define MATERIAL_H

enum MaterialType{
    plastic,emerald,ruby,gold,pearl
};

/**
 * @brief Holds the properties of all materials in MaterialType enum
 * 
 */
class Material{
    public:
        /**
         * @brief Construct a new Material object, default is plastic
         * 
         */
        Material();
        Material(MaterialType type);
        MaterialType type;
        float amb[4];
        float diff[4];
        float spec[4];
        float shiny[1];
};

#endif