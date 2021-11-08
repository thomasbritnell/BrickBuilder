#include <random>
#include <time.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <cmath>
#include <iostream>
#include "brick.h"

int n = 2;
int scale = 10;

bool left_mouse = false;
bool right_mouse = false;

bool space_down = false;

float eye_pos[] =  {21,5,21};
float eye_target[] = {0,0,0};
float eye_high[] = {0,1,0};


float angle = 0.01;

float grid_center;


std::vector<Brick> bricks;

//int old_mouse_pos[];
void kbd(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '1':
        n = 1;
        break;
    case '2':
        n = 2;
        break;
    case '3':
        n = 3;
        break;
    case '4':
        n = 4;
        break;
    case '+':
    if (scale < 25){
        scale++;
    }
        break;
    case '-':
    if ( scale > 2){
        
        scale--;
    }
        break;

    case 'c':
        eye_target[0] = grid_center;
        eye_target[1] = 0;
        eye_target[2] = grid_center;
        break;

    case 'q':
        exit(0);
        break;

    case 32:
        //space
        std::cout<< "space down" <<std::endl; 
        space_down = true;
    }
    
}
void kbd_up(unsigned char key, int x, int y){
    switch(key){
        case 32:
        std::cout<< "space up" <<std::endl; 
        space_down = false;
        break;
    }
}
void drawGrid(){
    glBegin(GL_LINES);

    glColor3f(1.0,1.0,1.0);
    //horizontal lines
    for (int i = 0; i <= scale; i++){
        glVertex3f(0.0, 0.0, i*n);
        glVertex3f(scale*n, 0.0, i*n);
    }

    //vertical lines
    for (int i = 0; i <= scale; i++){
        glVertex3f(i*n, 0.0, 0.0);
        glVertex3f(i*n, 0.0, scale*n);
    }
    glEnd();

    grid_center = scale*n/2;
}
void drawBricks(){
    for (Brick b : bricks){
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(b.position.mX,b.position.mY,b.position.mZ);
        glScalef(b.length, b.height, b.width); // stretch on X and Z
        glutSolidCube(n);
        glPopMatrix();
    }
    
}
void addBrick(float x, float y, float z){
    Colour c = {0.01*(rand()%100),0.01*(rand()%100),0.01*(rand()%100)};
    bricks.push_back(Brick(2,2,1,Point3D(x,y,z),0,c));
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye_pos[0], eye_pos[1], eye_pos[2], eye_target[0],eye_target[1],eye_target[2], eye_high[0],eye_high[1],eye_high[2]);
	
    //glRotatef(camera_rot[0],camera_rot[1],camera_rot[2],camera_rot[3]);

    drawBricks();
    drawGrid();

	glutSwapBuffers();
}
void update(int unused){

    // if( left_mouse){
    //         float x_prime = eye_pos[0] - grid_center;
    //         float z_prime = eye_pos[2] - grid_center;

    //         x_prime = x_prime * cos(angle) - z_prime * sin(angle);
    //         z_prime = z_prime * sin(angle) + z_prime * cos(angle);
           
    //         eye_pos[0] = x_prime + grid_center;
    //         eye_pos[2] = z_prime + grid_center;
    // }

    glutPostRedisplay();
    glutTimerFunc(17,update,0);
}

void init(void)
{

    srand(time(NULL));
	glClearColor(0.1,0.1,0.3,0);
	//glColor3f(0.2, 0.2, 1);
    eye_target[0] = grid_center;
    eye_target[1] = 0;
    eye_target[2] = grid_center;

    bricks = std::vector<Brick>();
    
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 500);
}


void mouse(int btn, int state, int x, int y)
{

    //std::cout << "mouse click: " << mouse_pos.mX << ", " << mouse_pos.mY << "\n";
    if (btn == GLUT_LEFT_BUTTON)
    {

        if (state == GLUT_UP)
        {
            left_mouse = false;

        }

        if (state == GLUT_DOWN)
        {
            left_mouse = true;
            addBrick(eye_target[0],eye_target[1],eye_target[2]);
            
        }
    }else if(btn == 3){//mouse scroll up
        if (state == GLUT_UP){return;}
        eye_pos[0] += 1;
        eye_pos[1] += 1;
        eye_pos[2] += 1;

    }else if(btn == 4){//mouse scroll down
        if (state == GLUT_UP){return;}
        eye_pos[0] -= 1;
        eye_pos[1] -= 1;
        eye_pos[2] -= 1;

    }
    // else if (btn == GLUT_RIGHT_BUTTON)
    // {
    //     if (state == GLUT_UP)
    //     {
    //         right_mouse = false;
    //     }

    //     if (state == GLUT_DOWN)
    //     {
    //         right_mouse = true;
    //     }
    // }
}


void motion(int x, int y){
    // if (left_mouse){
    //     eye_pos[0] = eye_pos[0] * cos(0.174533) - eye_pos[2]* sin(0.174533);
    //     eye_pos[1] = eye_pos[0] * sin(0.174533) - eye_pos[2]* cos(0.174533);
    // }else if (right_mouse){
    //     if (left_mouse){
    //     eye_pos[0] = eye_pos[0] * cos(-0.174533) - eye_pos[2]* sin(-0.174533);
    //     eye_pos[1] = eye_pos[0] * sin(-0.174533) - eye_pos[2]* cos(-0.174533);
    // } 
    // }

    // if (space_down){
    //     eye_target[0] = x;
    // }

}

int main(int argc, char **argv){
    srand(time(NULL));
    

    glutInit(&argc, argv);		//starts up GLUT
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    
	
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("Brick builder");	//creates the window

	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutMouseFunc(mouse);
    glutPassiveMotionFunc(motion);
    glutTimerFunc(17,update,0);
    glutKeyboardFunc(kbd);
    glutKeyboardUpFunc(kbd_up);

	glEnable(GL_DEPTH_TEST);
    
	glFrontFace(GL_CW);
	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);
    
	init();

	glutMainLoop();				//starts the event loop


    return 0;
}