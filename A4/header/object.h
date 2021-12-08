
#ifndef OBJECT_H
#define OBJECT_H

#include "mathLib3D.h"
#include "material.h"
#include "plane.h"
#include <limits>

enum ObjectType{
    cube, teapot, sphere, cone, torus, octahedron
};

/**
 * @brief Holds the information necessary to render objects in the scene
 * 
 */
class Object{
    public:
        /**
         * @brief Construct a new Object object, default is cube
         * 
         */
        Object();
        Object(Material material, ObjectType type);

        Point3D position;
        Point3D rotation;
        Point3D scale;
        Material material;
        ObjectType type;

        Point3D maxP, minP;
        Plane boundingPlanes[6];

        int maxScale;

        void calculateMaxScale();
        /**
         * @brief Sets up the bounding planes for an object
         * 
         */
        void allignBoundingPlanes();

        /**
         * @brief Checks ray box intersection test for the object
         * 
         * @param ray 
         * @return true intersection
         * @return false no intersection
         */
        bool rayBoxIntersection(Vec3D ray);

        Point3D closestRayIntercept;
};

#endif