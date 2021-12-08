#include "header/plane.h"

Plane::Plane(Point3D position, Vec3D normal) :
    position(position), normal(normal){}

Plane::Plane() : Plane(Point3D(), Vec3D()) {}

Point3D Plane::rayPlaneIntersection(Vec3D ray){
    float D = this->position.distanceTo(Point3D()); 
    float vectorsDotProd = Vec3D::dotProduct(this->normal,ray);
    float t = (-(Vec3D::dotProduct(this->normal,ray.start) + D)) / vectorsDotProd;

    float pX = ray.start.mX + t * ray.mX;
    float pY = ray.start.mY + t * ray.mY;
    float pZ = ray.start.mZ + t * ray.mZ;

    return Point3D(pX,pY,pZ);
}