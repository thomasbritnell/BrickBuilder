#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <iostream>
#include <stdlib.h>
#include "header/inputHandler.h"

Point3D InputHandler::initialRightClickPosition;
float InputHandler::previousRightClickPosition[2];
bool InputHandler::rightClick;
bool InputHandler::leftClick;


// std::stack<Command> InputHandler::actionStack = std::stack<Command>();
std::stack<PlaceBrickCommand> InputHandler::actionStack = std::stack<PlaceBrickCommand>();

//Command InputHandler::commandOutput;
//bool InputHandler::notify = false;


void InputHandler::mouse(int button, int state, int x , int y){
    y = 700 - y; // flip y

  //  std::cout << "Mouse Coordinates: ";
   // std::cout << x << ", " << y << std::endl; // endline is neccassary for terminal to print line by line


    if (button == GLUT_LEFT_BUTTON) // LMB pressed
    {
      //  std::cout << "LEFT BUTTON" << std::endl;
        if (state == GLUT_UP)
        {
     //       std::cout << "RELEASED!" << std::endl;
            leftClick = false;
        }
        if (state == GLUT_DOWN)
        {
            
      //      std::cout << "PRESSED!" << std::endl;
            leftClick = true;
        }
    }

    else if (button == GLUT_RIGHT_BUTTON) // RMB pressed
    {
      //  std::cout << "RIGHT BUTTON" << std::endl;
        if (state == GLUT_UP)
        {
       //     std::cout << "RELEASED!" << std::endl;
            rightClick = false;
            previousRightClickPosition[0] = camera->rotation[0]; // y position of mouse
            previousRightClickPosition[1] = camera->rotation[1]; // x position of mouse
        }
        if (state == GLUT_DOWN)
        {
          //  std::cout << "PRESSED!" << std::endl;
            rightClick = true;
            initialRightClickPosition = Point3D(x,y,0);

        }
    }

    else if(button == 3){//mouse scroll up
        if (state == GLUT_UP){return;}

//        std::cout << "SCROLL UP!" << std::endl;

        camera->zoomIn();

    }

    else if(button == 4){//mouse scroll down
        if (state == GLUT_UP){return;}

       // std::cout << "SCROLL DOWN!" << std::endl;

        camera->zoomOut();

    }
}

void InputHandler::motion(int x, int y){
    y = 700 - y; // flip y

    if(rightClick){
        Vec3D clickDistance = Vec3D::createVector(initialRightClickPosition, Point3D(x,y,0));
        camera->rotation[1] = clickDistance.mX + previousRightClickPosition[1]; // y axix rotation depends on x coords of mouse
        camera->rotation[0] = clickDistance.mY + previousRightClickPosition[0]; // x axis rotation depends on y coords of mouse
    }

    // // camera->rotation[0] = camera->rotation[0] % 360
    // std::cout << "rotation" << std::endl;
    // std::cout << camera->rotation[0] << std::endl;
    // std::cout << camera->rotation[1] << std::endl;

    // std::cout << x <<","<< y << std::endl;
    // std::cout << "Initial Click Position" << std::endl;
    // std::cout << initialRightClickPosition.mX << std::endl;
    // std::cout << initialRightClickPosition.mY << std::endl;
    // std::cout << "Distance" << std::endl;
    // std::cout << clickDistance.mX << std::endl;
    //     std::cout << clickDistance.mY << std::endl;
    // // std::cout << clickDistance.mX << std::endl;

}

/**
 * @brief returns the origin for now
 * 
 * @return Point3D 
 */
Point3D giveMousePos(){ // will return cursor position on board (using ray tracing)

//rand()%(max-min + 1) + min;
    int max_width = (InputHandler::board->width);
    int min_width = -(InputHandler::board->width);
    int max_length = (InputHandler::board->length);
    int min_length = -(InputHandler::board->length);


    return Point3D(rand()%(max_width-min_width) + min_width + 1,0,rand()%(max_length-min_length) + min_length).multiply(InputHandler::board->cellSize);
}

void InputHandler::keyboard(unsigned char key, int x, int y){
    switch (key){
        case 'q':
        exit(0);
        break;
        case 'a':
            // InputHandler::actionStack.pop();
            // PlaceBrickCommand command = PlaceBrickCommand(giveMousePos(),Brick(),InputHandler::board);
            actionStack.push(PlaceBrickCommand(Brick(giveMousePos(),0, static_cast<MaterialType>(rand()%3), MeshType::oneByOne),InputHandler::board));
            actionStack.top().execute();
            // command.execute();

            
        break;
        case 'u':
            if(!actionStack.empty()){
                actionStack.top().undo();
                actionStack.pop();
            }
        break;
        case '1':
            if(board->width < 20){
                board->width +=1;
            }
        break;
        case '2':
            if(board->width > 2){
                board->width -=1;
            }
        break;
        case '3':
            if(board->length < 20){
                board->length +=1;
            }
        break;
        case '4':
            if(board->length > 2){
                board->length -=1;
            }
        break;
        
    }
}

