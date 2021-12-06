

#ifndef CAMERA_H
#define CAMERA_H

#include "mathLib3D.h"
#include "vector"

class Camera{
    public:
        Camera();
        Camera(Point3D position, Point3D target, Vec3D upVector);

        Point3D position;
        Point3D target;
        Vec3D upVector;

        float rotation[3];

        void zoomIn();
        void zoomOut();
};

#endif