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
#include <stdlib.h>
#include <vector>

const int WINDOW_SIZE_X = 600;
const int WINDOW_SIZE_Y = 600;

bool drawing = false;

struct Colour
{
    float r, g, b;
};

struct Point
{
    int x, y;
    float size;
    Colour c;
};

std::vector<Point> points;

Colour currentColour = {0.0f, 0.0f, 1.0f};

int size = 3;

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    for (Point p : points)
    {
        glPointSize(p.size * 10.0f);
        glBegin(GL_POINTS);
        glColor3f(p.c.r, p.c.g, p.c.b);
        glVertex2f(p.x, p.y);
        glEnd();
    }

    glFlush();
    glutSwapBuffers();
}

void drawPoint(int x, int y)
{
    points.push_back({x, y, size, {currentColour.r, currentColour.g, currentColour.b}});
    glutPostRedisplay();
}

void mouse(int btn, int state, int x, int y)
{

    if (btn == GLUT_LEFT_BUTTON)
    {

        if (state == GLUT_UP)
        {
            drawing = false;
        }
        if (state == GLUT_DOWN)
        {
            drawing = true;
            drawPoint(x, WINDOW_SIZE_Y - y);
        }
    }
}

void motion(int x, int y)
{
    if (drawing)
    {
        drawPoint(x, WINDOW_SIZE_Y - y);
    }
}

void kbd(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '+':
        if (size < 5)
        {
            size++;
        }
        else
        {
            std::cout << "Max Size" << std::endl;
        }

        break;
    case '-':
        if (size > 1)
        {
            size--;
        }
        else
        {
            std::cout << "Min Size" << std::endl;
        }

        break;
    case 'R':
    case 'r':
        points.clear();
        glutPostRedisplay();

        break;
    case 'Q':
    case 'q':
        std::cout << "Quit" << std::endl;
        exit(0);
        break;

    case '1':
        std::cout << "colour 1" << std::endl;

        currentColour = {0.0f, 1.0f, 0.0f};
        break;
    case '2':
        std::cout << "colour 2" << std::endl;

        currentColour = {1.0f, 1.0f, 0.0f};
        break;
    case '3':
        std::cout << "colour 3" << std::endl;
        currentColour = {1.0f, 1.0f, 1.0f};
        break;
    case '4':
        std::cout << "colour 4" << std::endl;
        currentColour = {1.0f, 0.0f, 0.0f};
        break;
    case '5':
        std::cout << "colour 5" << std::endl;
        currentColour = {0.0f, 0.0f, 1.0f};
        break;
    case '6':
        std::cout << "colour 5" << std::endl;
        currentColour = {0.0f, 0.0f, 0.0f};
        break;
    }
}

/* main function - program entry point */
int main(int argc, char **argv)
{
    glutInit(&argc, argv); //starts up GLUT
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_SIZE_X, WINDOW_SIZE_Y);
    glutCreateWindow("Thomas Britnell Paint.exe"); //creates the window
    gluOrtho2D(0, WINDOW_SIZE_X, 0, WINDOW_SIZE_Y);

    glutKeyboardFunc(kbd);
    glutMotionFunc(motion);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return (0);
}
