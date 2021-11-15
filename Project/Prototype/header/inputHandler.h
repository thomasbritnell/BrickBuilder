


#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "camera.h"
#include "mathLib3D.h"
#include "material.h"
#include "mesh.h"
#include "vector"
#include "board.h"
#include "placeBrickCommand.h"
#include <stack>

class InputHandler{
    public:
        static Camera* camera;
        static Board* board;
        //static Command commandOutput;

        //static bool notify;
        
        // static std::stack<Command> actionStack;
        static std::stack<PlaceBrickCommand> actionStack;

        static void mouse(int button, int state, int x , int y);
        static void motion(int x, int y);
        static void keyboard(unsigned char key, int x, int y);
    
    private:
        static Point3D initialRightClickPosition;
        static float previousRightClickPosition[2];
        static bool rightClick;
        static bool leftClick;

};

#endif