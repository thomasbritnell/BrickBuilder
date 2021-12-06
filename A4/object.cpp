#include "header/object.h"

Object::Object(Material material, ObjectType type) :
    material(material), type(type) {
        position = Point3D();
        rotation = Point3D();
        scale = Point3D(1,1,1);
    } 
 
Object::Object() : Object(Material(), ObjectType::cube) {}