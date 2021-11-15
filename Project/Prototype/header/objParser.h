

#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include <stdio.h>
#include "mesh.h"
#include "mathLib3D.h"
#include "string"
#include "iostream"
#include <cstring>

class ObjParser{
    public:
        
        static Mesh loadMesh(char* file_path);
    private:
        static char* path;
};

#endif