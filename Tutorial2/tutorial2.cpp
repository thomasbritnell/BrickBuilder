/*
Thomas Britnell 
400129897
britnelt
September 28th 2021

*my partner is Eitan Yehuda, 
    but he had a religious holiday and was not present 
    for this tutorial. 
*/



#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
/* Global Variables */
float animateDistance = 0;
float pointX = 400;
float pointY = 400;

bool animate = false;
// bool rainbowmode =false; //bonus!

float r = 1.0f; // the default colour is white
float g = 1.0f;
float b = 1.0f;


/* Function which draws a point at x and y*/
void drawPoint(float x, float y)
{
    glPointSize(10);
   
    glColor3f(r,g,b);
    
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();

    glutPostRedisplay();

}

/*Write your Mouse code here*/
void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            animateDistance = 0; //reset the animation when click so it always starts again
            pointX = x;
            pointY = 800-y;//to flip the y based on the frame height
        }
    }
}

/*Write your Keyboard code here*/
void kbd(unsigned char button, int x, int y)
{
    if (button == 'p' || button == 'P'){
        animate = !animate;//toggle the animation
    }
    // else if (button == 32){//space bar
    //     rainbowmode = !rainbowmode;
    // }
    
}

/* Display Function */
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawPoint(pointX, pointY);//center

    drawPoint(pointX+animateDistance, pointY);//right
    drawPoint(pointX-animateDistance, pointY);//left
    drawPoint(pointX, pointY+animateDistance);//top
    drawPoint(pointX, pointY-animateDistance);//bottom
    glFlush();
}

/*Write your animation function here*/
void animation(int i){

    if (animate){
        if (animateDistance <= 50){//i assumed that the animation would just start over once it reached a certain distance
        //50 is arbitrary but looked good.
            animateDistance++;
        }else{
            animateDistance = 0;
        }
    }
    glutTimerFunc(17,animation,0);
}

//I tried to do a rainbow function but couldn't get it to work in time. So I have left it in case I revisit this.

// void rainbow(int i){
//     std::cout<<"switch colour!\n";
//     if (rainbowmode){
//     r = static_cast<float>(rand()%255+1);
//     g = static_cast<float>(rand()%255+1);
//     b = static_cast<float>(rand()%255+1);
//     }else{
//         r = 1.0f;
//         b = 1.0f;
//         g = 1.0f;
//     }
//     glutTimerFunc(170*6,rainbow,0);
// }


/* Initializer */
void init()
{
   // srand(time(NULL));
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("3GC3 Tutorial 2");
    glClearColor(0.0, 0.0, 0.0, 0);
    glColor3f(1.0, 1.0, 1.0);
    gluOrtho2D(0, 800, 0, 800);
}

/*Main*/
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    init();
    glutDisplayFunc(display);
    
    /* Write your call backs here */
    glutMouseFunc(mouse);
    glutTimerFunc(17,animation,0);
    // glutTimerFunc(60,rainbow,0);
    glutKeyboardFunc(kbd);

    glutMainLoop();
}
