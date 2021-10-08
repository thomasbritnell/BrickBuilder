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

const int INITIAL_POINT_COUNT = 100;


Point2D mouse_pos = Point2D(0,0);

bool left_mouse;
bool right_mouse;


bool running = true;

std::vector<Particle> particles;


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    for (Particle p : particles)
    {
        glPointSize(p.size * 2.0f);
        glBegin(GL_POINTS);
        glColor3f(p.colour.r,p.colour.g,p.colour.b);
        glVertex2f(p.position.mX, p.position.mY);
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
        float speed = (rand() % 5 + 1)*0.25;
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
        std::cout << "No particles to erase!\n";
    }
    

}

void modify_range(int delta){
    for(auto p = (&particles)->begin(); p != (&particles)->end(); ++p)
        {
            if (delta < 0){
            //subtracting from the range
                p->range = p->range*0.8;
            }else{
                p->range = p->range*1.2;
            }
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

bool in_range(Point2D position,float range){
    return position.distanceTo(mouse_pos) <= range;
}

void update_points(int x){
    if (running){
        for(auto p = (&particles)->begin(); p != (&particles)->end(); ++p)
        {

            Vec2D applied_force = Vec2D(0,0);
            Vec2D acceleration = Vec2D(0,0);
            Vec2D friction = Vec2D(0,0);

            if (left_mouse && in_range(p->position,p->range)){
                applied_force = Vec2D::createVector(mouse_pos,p->position).normalize().multiply(-0.5);
            }else if(right_mouse && in_range(p->position, p->range)){
                applied_force = Vec2D::createVector(mouse_pos,p->position).normalize().multiply(0.5);
            }


            if ((abs(p->direction.mX) >= 0.005) && (abs(p->direction.mY) >= 0.005)){
                friction = p->direction.normalize().multiply(-0.08/p->size);
            }else{
                p->direction = Vec2D(0,0);
            }

            acceleration = Vec2D(applied_force.mX+friction.mX, applied_force.mY+friction.mY);
            //std::cout<< "acceleration: " <<acceleration.mX << ", " << acceleration.mY << "\n";

            //janky 'vector add' function because i dont want to modify Vec2D itself
            p->direction = Vec2D(p->direction.mX + acceleration.mX, p->direction.mY + acceleration.mY);
            //std::cout<< "velocity: " << p->direction.mX << ", " << p->direction.mY << "\n";

            if (p->position.mX >= WINDOW_SIZE_X){
                p->direction = Vec2D(p->direction.mX*-1,p->direction.mY);
                p->position = Point2D(WINDOW_SIZE_X - p->size,p->position.mY);
            }else if (p->position.mX <= 0){
                p->direction = Vec2D(p->direction.mX*-1,p->direction.mY);
                p->position = Point2D(0 + p->size,p->position.mY);
            }

            if (p->position.mY >= WINDOW_SIZE_Y){
                p->direction = Vec2D(p->direction.mX,p->direction.mY*-1);
                p->position = Point2D(p->position.mX,WINDOW_SIZE_Y - p->size);
            }else if (p->position.mY <= 0){
                p->direction = Vec2D(p->direction.mX,p->direction.mY*-1);
                p->position = Point2D(p->position.mX,0 + p->size);
            }


            p->position = p->direction.movePoint(p->position);
        }
    }
    //glutPostRedisplay();
    glutTimerFunc(17,update_points,0);
}

void mouse(int btn, int state, int x, int y)
{
    mouse_pos.mX = static_cast<float>(x);
    mouse_pos.mY = static_cast<float>(WINDOW_SIZE_Y - y);

    std::cout<< "mouse click: "<< mouse_pos.mX << ", " <<mouse_pos.mY <<"\n";
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
    mouse_pos.mX = static_cast<float>(x);
    mouse_pos.mY = static_cast<float>(WINDOW_SIZE_Y - y);
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
            modify_range(1);
        break;
    case '-':
            std::cout << "decrease range\n";
            modify_range(-1);
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
    glutMotionFunc(motion);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return (0);
}
