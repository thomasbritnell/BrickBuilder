#include "header/fileHandler.h"


/*
Files are formatted as follows:

num_of_objects
light1_x
light1_y
light1_z
light2_x
light2_y
light2_z
object1_x
object1_y
object1_z
object1_rot_x
object1_rot_y
object1_rot_z
object1_scale_x
object1_scale_y
object1_scale_z
object1_material
object1_type
.
.
.
objectN_x
objectN_y
objectN_z
objectN_rot_x
objectN_rot_y
objectN_rot_z
objectN_scale_x
objectN_scale_y
objectN_scale_z
objectN_material
objectN_type
*/


void FileHandler::saveFile(std::string fileName, float* data, int size){

    std::ofstream File (fileName);


    if ( File.is_open()){
        for (int i = 0; i < size; i++){
            try{
                File << data[i] << std::endl;
            }catch(...){
                std::cout<<"Error writing to file: "<< i <<std::endl;
                break;
            }
        }
    }

    File.close();
}

void FileHandler::loadFile(std::string fileName, float* data, int& objectCount){
    
    std::ifstream File;

    File.open(fileName, std::ios_base::in);
    
    

    for (int i = 0; i < 7;i++){
        File >> data[i];
    }

    objectCount = data[0];


    for (int i = 7; i < (objectCount*11)+7;i++){
        File >> data[i];
    }

    File.close();

}