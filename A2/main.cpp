/*
    Thomas Britnell
    400129897
    britnelt
   Assignment 2
*/

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include "particle.h"
#include <time.h>

//initial window size
const int WINDOW_SIZE_X = 600;
const int WINDOW_SIZE_Y = 600;

const int INITIAL_POINT_COUNT = 10;


int mouse_x;
int mouse_y;
bool left_mouse;
bool right_mouse;

bool running = true;

std::vector<Particle> particles;


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    for (Particle p : particles)
    {
        glPointSize(p.size * 1.5f);
        glBegin(GL_POINTS);
        glColor3f(p.colour.r,p.colour.g,p.colour.b);
        glVertex2f(p.position.mX, p.position.mY);
        //std::cout<< p.position.mX << ", " << p.position.mY << std::endl;
        glEnd();
    }

    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
}

void add_point(float x, float y){
        float r = (rand() % 100 + 0)*0.01;
        float g = (rand() % 100 + 0)*0.01;
        float b = (rand() % 100 + 0)*0.01;
        int size = rand() % 5 + 1;
        float x_direction = 0 ; //(rand() % 2 - 1);
        float y_direction = 0 ; //(rand() % 2 - 1);
        float range = rand() % WINDOW_SIZE_X/2 + 10;
        float speed = rand() % 5 + 1;
        particles.push_back(
            Particle(Point2D(x,y),{r,g,b},size,Vec2D(x_direction,y_direction),range,speed)
        );
}

void remove_point(float x, float y){
   
   std::vector<Particle>::iterator nearest;

    Point2D mouse_pos = Point2D(x,y);

    float distance = INT_MAX;
    
   for(auto p = (&particles)->begin(); p != (&particles)->end(); ++p)
        {
            if (p->position.fastDistanceTo(mouse_pos) <= distance){
                distance = p->position.fastDistanceTo(mouse_pos);
                nearest = p;
            }
        }
    if (particles.size() > 0){   
        particles.erase(nearest);
    }else{
        std::cout << "No praticles to erase!\n";
    }
    

}

void generatePoints()
{
    particles.clear();
    for(int i = 0; i < INITIAL_POINT_COUNT; i++){
        float x = rand() % WINDOW_SIZE_X + 0;
        float y = rand() % WINDOW_SIZE_Y + 0;
        add_point(x,y);
    }
}

//returns if the particle is in the range of the mouse
bool in_range(Point2D position,float range){
    return ((position.mX+range >= mouse_x) || (position.mX-range <= mouse_x) &&
            (position.mY+range >= mouse_y) || (position.mY-range <= mouse_y));
}

void update_points(int x){
    if (running){
        for(auto p = (&particles)->begin(); p != (&particles)->end(); ++p)
        {
            // if (left_mouse && in_range(p->position,p->range)){
            //     p->direction = Vec2D(1.0,0);
            // }
            //std::cout << " Old: " <<p->position.mX << ", " << p->position.mY << "\n";
            p->position = p->direction.multiply(p->speed).movePoint(p->position);
            //p->direction = Vec2D(p->direction.mX + ,p->direction.mY + );
            //std::cout << " New: " <<p->position.mX << ", " << p->position.mY << "\n";
        }
    }
    //glutPostRedisplay();
    glutTimerFunc(17,update_points,0);
}

void mouse(int btn, int state, int x, int y)
{
    if (btn == GLUT_LEFT_BUTTON)
    {
        
        if (state == GLUT_UP)
        {
            left_mouse = false;
        }
       
        if (state == GLUT_DOWN)
        {
            left_mouse =true;
        }
    }else if(btn == GLUT_RIGHT_BUTTON){
        if (state == GLUT_UP)
        {
            right_mouse = false;
        }
        
        if (state == GLUT_DOWN)
        {
            right_mouse = true;
        }
    }
}


void motion(int x, int y)
{

}


void kbd(unsigned char key, int x, int y)
{
    switch (key)
    {
        //increases the size of the points only if it is less than the max
    case 'a':
            std::cout << "add particle at:" << x <<", "<<y << "\n"; 
            add_point(static_cast<float>(x),static_cast<float>(WINDOW_SIZE_Y - y));

        break;
    case 'd':
            std::cout << "remove particle\n"; 
            remove_point(static_cast<float>(x),static_cast<float>(WINDOW_SIZE_Y - y));
        break;
    case '+':
            std::cout << "increase range\n"; 
        break;
    case '-':
            std::cout << "decrease range\n";
        break;
    case 32:
        std::cout << "Pause simulation\n";
        running = !running;
        break;
    case 'r':
        std::cout << "Reset scene\n";
        generatePoints();
        break;
    case 'q':
        std::cout << "quit the program";
        exit(0);
        break;
    }
}

/* main function - program entry point */
int main(int argc, char **argv)
{

    srand (time(NULL));
    generatePoints();
    glutInit(&argc, argv); //starts up GLUT
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_SIZE_X, WINDOW_SIZE_Y);
    glutCreateWindow("Particle Simulation.exe"); //creates the window
    gluOrtho2D(0, WINDOW_SIZE_X, 0, WINDOW_SIZE_Y);
    glutTimerFunc(17,update_points,0);
    glutKeyboardFunc(kbd);
    //glutMotionFunc(motion);
    glutDisplayFunc(display);
    //glutMouseFunc(mouse);
    glutMainLoop();
    return (0);
}
