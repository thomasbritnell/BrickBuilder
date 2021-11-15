#ifndef MESH_H
#define MESH_H

#include <vector>

#include "mathLib3D.h"

class Mesh{
    public:
        Mesh();
        std::vector<Point3D> vertices;
        std::vector<Vec3D> normals;
};

#endif 