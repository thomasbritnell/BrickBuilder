
#ifndef OBJECT_H
#define OBJECT_H

#include "mathLib3D.h"
#include "material.h"
#include "plane.h"

enum ObjectType{
    cube, teapot, sphere, cone, torus, octahedron
};


class Object{
    public:
        Object();
        Object(Material material, ObjectType type);

        Point3D position;
        Point3D rotation;
        Point3D scale;
        Material material;
        ObjectType type;
        Plane boundingPlanes[6];
};

#endif