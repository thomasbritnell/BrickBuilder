
#ifndef PLANE_H
#define PLANE_H

#include "mathLib3D.h"

class Plane{
    public:
        Plane();
        Plane(Point3D position, Vec3D normal);

        Point3D position;
        Vec3D normal;
};

#endif