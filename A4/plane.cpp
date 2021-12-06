#include "header/plane.h"

Plane::Plane(Point3D position, Vec3D normal) :
    position(position), normal(normal){}

Plane::Plane() : Plane(Point3D(), Vec3D()) {}