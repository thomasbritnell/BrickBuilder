#include "header/object.h"

Object::Object(Material material, ObjectType type) :
    material(material), type(type) {
        position = Point3D();
        rotation = Point3D();
        scale = Point3D(1,1,1);
        maxP = Point3D(1.5,1.5,1.5);
        minP = Point3D(-1.5,-1.5,-1.5);
        maxScale = 1;
    }
 
Object::Object() : Object(Material(), ObjectType::cube) {}

void Object::calculateMaxScale(){
    maxScale = this->scale.mX;
    if(this->scale.mY > maxScale){
        maxScale = this->scale.mY;
    }
    if(this->scale.mZ > maxScale){
        maxScale = this->scale.mZ;
    }
}

void Object::allignBoundingPlanes(){

    maxP = Point3D(1.5*(maxScale*0.9)+position.mX,1.5*(maxScale*0.9)+position.mY,1.5*(maxScale*0.9)+position.mZ);
    minP = Point3D(-1.5*(maxScale*0.9)+position.mX,-1.5*(maxScale*0.9)+position.mY,-1.5*(maxScale*0.9)+position.mZ);
    
    boundingPlanes[0] = Plane(Point3D(maxP.mX,0,0),Vec3D(1,0,0));
    boundingPlanes[1] = Plane(Point3D(0,maxP.mY,0),Vec3D(0,1,0));
    boundingPlanes[2] = Plane(Point3D(0,0,maxP.mZ),Vec3D(0,0,1));
    boundingPlanes[3] = Plane(Point3D(minP.mX,0,0),Vec3D(-1,0,0));
    boundingPlanes[4] = Plane(Point3D(0,minP.mY,0),Vec3D(0,-1,0));
    boundingPlanes[5] = Plane(Point3D(0,0,minP.mZ),Vec3D(0,0,-1));
}

bool Object::rayBoxIntersection(Vec3D ray){
    bool hit = false;
    for (int i = 0; i<6; i++){
        float closestDist = std::numeric_limits<float>::max();
        Point3D intercept = boundingPlanes[i].rayPlaneIntersection(ray);
        // if(rayIntercept.mX =NULL){
        //     return false;
        // }
        if(i == 0 || i == 3){// x plane
            if(this->minP.mY < intercept.mY && intercept.mY < this->maxP.mY && this->minP.mZ < intercept.mZ && intercept.mZ < this->maxP.mZ){
                if(ray.start.distanceTo(intercept) < closestDist){
                    closestRayIntercept = intercept;
                    closestDist = ray.start.distanceTo(intercept);
                    hit = true;
                }
            }
        } 
        else if(i == 1 || i == 4){// y plane
            if(this->minP.mX < intercept.mX && intercept.mX < this->maxP.mX && this->minP.mZ < intercept.mZ && intercept.mZ < this->maxP.mZ){
                if(ray.start.distanceTo(intercept) < closestDist){
                    closestRayIntercept = intercept;
                    closestDist = ray.start.distanceTo(intercept);
                    hit = true;

                }
            }
        } 
        else if(i == 2 || i == 5){// z plane
            if(this->minP.mY < intercept.mY && intercept.mY < this->maxP.mY && this->minP.mX < intercept.mX && intercept.mX < this->maxP.mX){
                if(ray.start.distanceTo(intercept) < closestDist){
                    closestRayIntercept = intercept;
                    closestDist = ray.start.distanceTo(intercept);
                    hit = true;
                }
            }
        } 
    }
    return hit;
}