/*
    Thomas Britnell
    400129897
    britnelt
    Simple paint program using OpenGl and GLUT
*/

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <iostream>
#include <stdlib.h>
#include <vector>


const int WINDOW_SIZE_X = 600;
const int WINDOW_SIZE_Y = 600;

//to keep track of if the mouse is down
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

/*
List of the current points to be be displayed to the screen.
When the user draws, points get added to this vector.
When the user clears, this vector is emptied.
*/
std::vector<Point> points;

//default colour is blue
Colour currentColour = {0.0f, 0.0f, 1.0f};

//default point size for drawing
int size = 3;

/*
registered below as the glutDisplayFunc.

Iterates through a vector of points and draws them to the screen.
*/
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

/*
Adds the passed coordinates to the vector of points to be drawn to the screen,
then prompts the glutDisplayFunc (via glutPostRedisplay).
*/
void drawPoint(int x, int y)
{
    points.push_back({x, y, size, {currentColour.r, currentColour.g, currentColour.b}});
    glutPostRedisplay();
}

/*
Toggles drawing mode based on if the left mouse button is pressed or not.
Registered below as glutMouseFunc.

*/
void mouse(int btn, int state, int x, int y)
{

    if (btn == GLUT_LEFT_BUTTON)
    {

        if (state == GLUT_UP)
        {
            drawing = false;
        }
        //When the mouse is clicked, draws a single point there
        if (state == GLUT_DOWN)
        {
            drawing = true;
            //y is flipped because glut interprets the window y opposite of how it is reported by the mouse event
            drawPoint(x, WINDOW_SIZE_Y - y);
        }
    }
}

/*
Registered as glutMotionFunc.
Draws a continuous line of points while drawing (the left mouse button is clicked)
*/
void motion(int x, int y)
{
    if (drawing)
    {
        //y is flipped because glut interprets the window y opposite of how it is reported by the mouse event
        drawPoint(x, WINDOW_SIZE_Y - y);
    }
}

/*
Registered below as glutKeyboardFunction.
Handles all the keyboard inputs. 
*/
void kbd(unsigned char key, int x, int y)
{
    switch (key)
    {
        //increases the size of the points only if it is less than the max
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
        //decrements the size of the points only if it is greater than 1 
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
        points.clear();//clear the list of points to be displayed
        glutPostRedisplay();//refresh

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
