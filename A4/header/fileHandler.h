
#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <iostream>
#include <fstream>
#include <string.h>

class FileHandler{

    public:
        static void loadFile(std::string filename, float* data, int& size);
        static void saveFile(std::string filename, float* data, int size);

};

#endif