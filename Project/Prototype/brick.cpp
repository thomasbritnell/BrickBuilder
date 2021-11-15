#include "header/brick.h"


Brick::Brick(Point3D position, int orientation, MaterialType material, MeshType meshType):
    position(position), orientation(orientation),material(material), meshType(meshType){}

Brick::Brick() : Brick(Point3D(),0,MaterialType::plastic,MeshType::twoByThree){}
            
