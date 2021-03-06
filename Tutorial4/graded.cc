
#ifdef __APPLE__
#  define GL_SILENCE_DEPRECATION
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif
#include <stdio.h>
#include <stdlib.h>


//Thomas Britnell and Eitan Yehuda
/*
Group 3
November 2nd
uuh idk what else to write here. 
*/




GLdouble eye[] = { 0, 0, 15 };
GLdouble lookAt[] = { 0, 0, 0 };
GLdouble up[] = { 0, 1, 0 };

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(
        eye[0], eye[1], eye[2],
        lookAt[0], lookAt[1], lookAt[2],
        up[0], up[1], up[2]
    );

    /**
     * Blue teapot (1).
     */
    glColor3f(0, 0, 1);
    glPushMatrix();
        glTranslatef(0, 0, -10);
        glRotatef(180, 1, 0, 1);
        glutWireTeapot(1);
    glPopMatrix();

    /**
     * Green teapot (2).
     */
    glColor3f(0, 1, 0);
    glPushMatrix();
        glTranslatef(2, -2, -5);
        glRotatef(90, 0, 0, 1);
        glutWireTeapot(1);
    glPopMatrix();

    /**
     * Red teapot (3).
     */
    // Write your code here!


    glColor3f(1, 0, 0);
    glPushMatrix();

        glTranslatef(0,0,-30);
        glRotatef(45,0,0,1);
        glutWireTeapot(1);
    glPopMatrix();

    glFlush();
}

void handleReshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    /**
     * Call gluPerspective here.
     */
    gluPerspective(45, 1, 1, 25);//shortened the clipping plane to hide the red teapot for bonus marks
    //gluPerspective(45,1,1,100);

    glMatrixMode(GL_MODELVIEW);
}

void handleKeyboard(unsigned char key, int _x, int _y) {
    if (key == 'q') {
        exit(0);
    }

    if (key == '0') {
        /** Reset the gluLookAt vectors.
         */
        eye[0] = 0;
        eye[1] = 0;
        eye[2] = 15;
        lookAt[0] = 0;
        lookAt[1] = 0;
        lookAt[2] = 0;
        up[0] = 0;
        up[1] = 1;
        up[2] = 0;
        display();
    } else if (key == '1') {
        // Write your code here!
       
        eye[0] = 5;
        eye[1] = 0;
        eye[2] = -10;
        lookAt[0] = 0;
        lookAt[1] = 0;
        lookAt[2] = -10;
        up[0] = 0;
        up[1] = -1;
        up[2] = 0;
        display();
    } else if (key == '2') {
        // Write your code here!
       
        eye[0] = 2;
        eye[1] = -2;
        eye[2] = 0;
        lookAt[0] = 2;
        lookAt[1] = -2;
        lookAt[2] = -5;
        up[0] = -0.5;
        up[1] = 0;
        up[2] = 0;
        display();
    } else if (key == '3') {
        
        eye[0] = 0;
        eye[1] = 0;
        eye[2] = -25;
        lookAt[0] = 0;
        lookAt[1] = 0;
        lookAt[2] = -30;
        up[0] = -1;
        up[1] = 1;
        up[2] = 0;
        display();
    }
}

/*Added functionality for moving the camera with the arrow keys
*/
void specialKeyboard(int key, int x, int y){
    switch (key){
        case GLUT_KEY_UP:
            lookAt[1]++;
        break;

        case GLUT_KEY_DOWN:
            lookAt[1]--;
        break;

        case GLUT_KEY_LEFT:
            lookAt[0]--;
        break;

        case GLUT_KEY_RIGHT:
            lookAt[0]++;
        break;
    }
    display();

}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600,600);
    glutInitWindowPosition(300,300);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Tutorial 3");

    glutKeyboardFunc(handleKeyboard);
    glutSpecialFunc(specialKeyboard);
    glutReshapeFunc(handleReshape);
    glutDisplayFunc(display);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glutMainLoop();

    return 0;
}
