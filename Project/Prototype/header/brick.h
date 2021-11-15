
#ifndef BRICK_H
#define BRICK_H

#include "mathLib3D.h"
#include "material.h"
#include "meshType.h"


class Brick{
    public:
        Brick();
        Brick(Point3D position, int orientation, MaterialType material, MeshType meshType);
        Point3D position;
        int orientation; // could be an enum N,S,E,W
        MeshType meshType;
        MaterialType material;
};

#endif
