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
#include <vector>
#include <math.h>
#include <stack>


#include "header/board.h"
#include "header/camera.h"
#include "header/inputHandler.h"
#include "header/objParser.h"

int windowSizeX = 700;
int windowSizeY = 700;


char oneByOnePath[] = "ObjectFiles/oneByOne.obj";
char twoByThreePath[] = "ObjectFiles/twoByThree.obj";

Board board;
Camera camera;

Camera* InputHandler::camera;
Board* InputHandler::board;


void printInstructions(){

    char inst[] = "Press 1 and 2 to increase/decrease the grid width. \n"
                "Press 3 and 4 to increase/decrease the grid length.\n"
                "Use the scroll wheel to zoom in and out.\n"
                "Press a to add a brick at a random location.\n"
                "Press u to undo a brick placement. \n"
                "Right click and drag with the mouse to rotate the scene.\n"
                "Press q to quit. \n";

    std::cout<<inst<<std::endl;
}

/* init function - initializes everything at the begining */
void init(void){
    Point3D orgin = Point3D(0,0,0); 
    // glEnable(GL_LIGHTING);
    // glEnable(GL_LIGHT0);
    // glEnable(GL_LIGHT1);
    
    board = Board(orgin, 5, 10, 10, std::vector<Brick>());
    camera = Camera(Point3D(0,100,100), orgin, Vec3D(0,1,0));
    InputHandler::camera = &camera;
    InputHandler::board = &board;

	glClearColor(0.2, 0.2, 0.2, 0); // background colour
    glColor3f(1, 1, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// glOrtho(-100, 100, -100, 100, -500, 500);
    gluPerspective(45, 1, 1, 5000);


   
    printInstructions();
}

void drawGrid(Point3D center,int cellSize, int length, int width){

    glLineWidth(2);
    glPushMatrix();
    glTranslatef(center.mX,center.mY,center.mZ);

    glBegin(GL_LINES);

    glColor3f(1.0,1.0,1.0); // line colour

    // horizontal lines
    for (int i = -length; i <= length; i++){
        glVertex3f(-width*cellSize, 0, i*cellSize);
        glVertex3f(width*cellSize, 0, i*cellSize);
    }
    // vertical lines
    for (int i = -width; i <= width; i++){
        glVertex3f(i*cellSize, 0, -length*cellSize);
        glVertex3f(i*cellSize, 0, length*cellSize);
    }

    glEnd();

    // glPushMatrix();

    // glTranslatef(board.cursorPos[0]*board.cellSize,board.cursorPos[1],board.cursorPos[2]*board.cellSize);
    // glColor3f(1.0,0,0);
    // glutWireCube(board.cellSize);

    // glPopMatrix();
    
    glPopMatrix();

  
}


void drawMesh(MeshType meshType, MaterialType materialType){
    //std::cout<<"got to daw mesh"<<std::endl;

    Mesh m;
    Material material = Material(materialType);

    switch (meshType){
        case 0:// 1 x 1
            m = ObjParser::loadMesh(oneByOnePath);
        break;
        case 1:// 2 x 3
            m = ObjParser::loadMesh(twoByThreePath);
        break;
        default:// 2 x 3
            m = ObjParser::loadMesh(twoByThreePath);
        break;
    }
    
    
    glBegin(GL_TRIANGLES);
        glColor3f(material.diff[0],material.diff[1],material.diff[2]);
        for (Point3D p : m.vertices){
            glVertex3f(p.mX,p.mY,p.mZ);
        }
    glEnd();
    
    }

void drawBricks(){


    for (Brick b:board.bricks){

        glPushMatrix();

        glTranslatef(b.position.mX,b.position.mY,b.position.mZ);
        glRotatef(b.orientation*180,0,1,0);

        //glScalef(b.length, b.height, b.width); // getting rid of this is lieu of having different meshes
        //glutSolidCube(board.cellSize);
        
        
        drawMesh(b.meshType,b.material);
        
        glPopMatrix();
    }
       
}


void drawBoard(Board* board){
    glPushMatrix();
    glRotatef(-camera.rotation[0], 1,0,0);
    glRotatef(camera.rotation[1], 0,1,0);

    drawGrid(board->center, board->cellSize, board->length, board->width);
    drawBricks();
   // drawMesh();

    glPopMatrix();
}



/* display function - draws everything */
void display(void){   

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    
    gluLookAt(camera.position.mX, camera.position.mY, camera.position.mZ, camera.target.mX, camera.target.mY, camera.target.mZ, camera.upVector.mX, camera.upVector.mY, camera.upVector.mZ);

    // gluLookAt(100, 100, 0, 0, 0, 0, 0, 1, 0);
    
    drawBoard(&board);

    // glFlush();
	glutSwapBuffers();

}

void update(int unused){
    

    glutPostRedisplay();
    glutTimerFunc(17,update,0);
}


/* main function - program entry point */
int main(int argc, char** argv){
    srand(time(NULL));

	glutInit(&argc, argv); // starts up GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH ); // Sets dispaly mode to double,rgb,depth


	glutInitWindowSize(windowSizeX, windowSizeY);
    glutInitWindowPosition(0, 0);

	glutCreateWindow("Brick Builder");	// creates the window

	glutDisplayFunc(display);	// registers "display" as the display callback function
	// glutReshapeFunc(reshape);
    glutKeyboardFunc(InputHandler::keyboard);
	// glutSpecialFunc(special);
	glutMouseFunc(InputHandler::mouse);
	glutMotionFunc(InputHandler::motion);
	
	// listOfCommands(); // print out command options

	// glutTimerFunc(1000.0/60.0, update, 0); // timer
	glutTimerFunc(17, update, 0); // timer

    glEnable(GL_DEPTH_TEST);  // culling
	glFrontFace(GL_CW);
	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);

    init();

	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}