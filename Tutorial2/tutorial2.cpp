#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#endif
#include <iostream>
/* Global Variables */
float animateDistance = 0;
float pointX = 400;
float pointY = 400;

bool animate = false;

/* Function which draws a point at x and y*/
void drawPoint(float x, float y)
{
    glPointSize(10);
    glColor3f(1.0f, 1.0f, 1.0f);

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
            animateDistance = 0; //reset the animation
            pointX = x;
            pointY = 800-y;//to flip the y based on the frame height
        }
    }
}

/*Write your Keyboard code here*/
void kbd(unsigned char button, int x, int y)
{
    if (button == 'p' || button == 'P'){
        animate = !animate;
    }
}

/* Display Function */
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawPoint(pointX, pointY);//center

    drawPoint(pointX+animateDistance, pointY);//right
    drawPoint(pointX-animateDistance, pointY);//left
    drawPoint(pointX, pointY+animateDistance);//bottom
    drawPoint(pointX, pointY-animateDistance);//top
    glFlush();
}

/*Write your animation function here*/
void animation(int i){

    if (animate){
        if (animateDistance <= 50){
            animateDistance++;
        }else{
            animateDistance = 0;
        }
    }
    glutTimerFunc(17,animation,0);
}
/****/

/* Initializer */
void init()
{
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
    glutKeyboardFunc(kbd);

    glutMainLoop();
}
