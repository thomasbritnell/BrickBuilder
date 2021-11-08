#include "headers/particleList.h"
#include <random>
#include <time.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <iostream>

int window_size_x = 800;
int window_size_y = 800;

float globalRot[] = {0, 0, 0}; // rotation of the whole scene. Everything moves while the camera is stationary.

int CAMPOS[3] = {300, 300, 0};                                                                                   // the default camera position
float camPos[3] = {static_cast<float>(CAMPOS[0]), static_cast<float>(CAMPOS[1]), static_cast<float>(CAMPOS[2])}; // the actual camera position
float lookAt[3];                                                                                                 // actual camera target
const int LOOKAT[3] = {0, 0, 0};                                                                                 // default camera target
float lightPos[2][4] = {{0, 100, -100, 1}, {0, 100, 100, 1}};                                                    // intiial positions for both lights

int selectedLight = 0;     // currently selected light of the two available
float cannonRot[2] = {0, 0}; // x and z plane rotation of the particle cannon

bool fountain_on = true;
bool friction_mode = true;

bool lights_on = true;

bool particleCam = false;        // used to toggle the bonus particle cam mode
Particle3D selectedParticleCopy; // the followed particle when the particle camera mode is active

// The values for the lights
float light_amb[4] = {1, 1, 1, 1};
float light_dif[4] = {1, 1, 1, 1};
float light_spc[4] = {1, 1, 1, 1};

Point3D origin;             // origin of the particle flow
ParticleList particle_list; // the list of all particles in the system

/*
Draws the lights based on their current position as specified by the user.

*/
void drawLight()
{

    if (lights_on)
    {
        // Obsidian
        //  float amb[] = { 0.15375f, 0.15f, 0.06625f, 0.82f };
        //  float diff[] = { 0.18275f, 0.17f, 0.22525f, 0.82f};
        //  float spec[] = {0.332741f, 0.328634f, 0.346435f, 0.82f };
        float shin = 38.4f;

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, light_amb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, light_dif);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, light_spc);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shin);
    }
    else
    {
        glColor3f(0.9f, 0.9f, 0.9f);
    }

    glPushMatrix();
    // glLoadIdentity();
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos[0]);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_dif);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_spc);

    glTranslatef(lightPos[0][0], lightPos[0][1], lightPos[0][2]);
    glutSolidTeapot(5);

    glPopMatrix();

    glPushMatrix();
    // glLoadIdentity();
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos[1]);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_dif);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_amb);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_spc);

    glTranslatef(lightPos[1][0], lightPos[1][1], lightPos[1][2]);
    glutSolidTeapot(5);

    glPopMatrix();
}

/*
Draws the "floor" on which the particles bounce. Also includes a grid to help with scale.
*/
void drawFloor()
{

    float size = 1000.0f;
    int num_lines = 100;
    float n = size / num_lines;

    glPushMatrix();
    if (lights_on)
    {
        // Obsidian
        float amb[] = {0.05375f, 0.05f, 0.06625f, 0.82f};
        float diff[] = {0.18275f, 0.17f, 0.22525f, 0.82f};
        float spec[] = {0.332741f, 0.328634f, 0.346435f, 0.82f};
        float shin = 38.4f;

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shin);
    }
    else
    {
        glColor3f(0.1f, 0.1f, 0.1f);
    }

    Vec3D normal = Vec3D::crossProduct(Vec3D::createVector(Point3D(-size, -10.0f, size), Point3D(-size, -10.0f, -size)),
                                       Vec3D::createVector(Point3D(size, -10.0f, -size), Point3D(-size, -10.0f, -size)))
                       .normalize()
                       .multiply(-1);

    glBegin(GL_QUADS);
    glNormal3f(normal.mX, normal.mY, normal.mZ);
    glVertex3f(-size, -10.0f, -size); // v1
    glNormal3f(normal.mX, normal.mY, normal.mZ);
    glVertex3f(-size, -10.0f, size); // v2
    glNormal3f(normal.mX, normal.mY, normal.mZ);
    glVertex3f(size, -10.0f, size); // v4
    glNormal3f(normal.mX, normal.mY, normal.mZ);
    glVertex3f(size, -10.0f, -size); // v3
    glEnd();

    if (lights_on)
    {
        // Obsidian
        float amb[] = {0.05375f, 0.05f, 0.06625f, 0.82f};
        float diff[] = {0.18275f, 0.17f, 0.22525f, 0.82f};
        float spec[] = {0.332741f, 0.328634f, 0.346435f, 0.82f};
        float shin = 38.4f;

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shin);
    }
    else
    {
        glColor3f(1.0f, 1.0f, 1.0f);
    }

    // begin the grid
    glLineWidth(2);
    glBegin(GL_LINES);
    // horizontal lines
    for (int i = -num_lines; i <= num_lines; i++)
    {
        glVertex3f(-num_lines * n, -9.0f, i * n);
        glVertex3f(num_lines * n, -9.0f, i * n);
    }
    // vertical lines
    for (int i = -num_lines; i <= num_lines; i += 2)
    {
        glVertex3f(i * n, -9.0f, -num_lines * n);
        glVertex3f(i * n, -9.0f, num_lines * n);
    }

    glEnd();

    glPopMatrix();
}

/*
glutTimerFunc, called 60 times per second. Updates all the particles so that their states change continuosly.
*/
void update(int unused)
{

    particle_list.updateParticles(friction_mode);

    if (particleCam)
    {
        (&selectedParticleCopy)->update(friction_mode);
    }

    glutPostRedisplay();
    glutTimerFunc(17, update, 0);
}

/*
Helper function to reutn a random value in [min,max)
*/
float random(int min, int max)
{
    return rand() % (max - min) + min;
}

/*
Creates and returns a new particle based on random parameters.
*/
Particle3D createParticle(Point3D origin)
{
    Point3D position = origin;
    Vec3D direction = Vec3D(random(-50, 51) * particle_list.direction.mX + cannonRot[1] / (45), particle_list.direction.mY, random(-50, 51) * particle_list.direction.mZ + cannonRot[0] / (45));
    int speed = random(5, 8);
    Angle rotation = {random(0, 360), random(0, 360), random(0, 360)};
    int size = random(4, 7);

    MaterialType m = static_cast<MaterialType>(random(0, 3));
    // std::cout<< m << std::endl;

    Material material = Material(m);

    Colour colour = {material.diff[0], material.diff[1], material.diff[2]};

    int age = 0;

    Particle3D newParticle = Particle3D(position, direction, speed, rotation, size, colour, material, age);

    return newParticle;
}

/**
 * Generates an initial list of particles based around the origin.
 *
 */
ParticleList generateParticles(Point3D origin)
{

    std::vector<Particle3D> particles;

    for (int i = 0; i < ParticleList::BURST_SIZE; i++)
    {

        Particle3D new_particle = createParticle(origin);

        particles.push_back(new_particle);
    }
    ParticleList list = ParticleList(particles, origin);
    return list;
}

/**
 * Draws the cannon to the screen based on its rotation, as specified by the user.
 *
 */
void drawCannon()
{
    glPushMatrix();
    if (lights_on)
    {
        // Tin
        float amb[] = {0.105882f, 0.058824f, 0.113725f, 1.0f};
        float diff[] = {0.427451f, 0.470588f, 0.541176f, 1.0f};
        float spec[] = {0.333333f, 0.333333f, 0.521569f, 1.0f};
        float shine = 9.84615f;

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shine);
    }
    else
    {
        glColor3f(0.427451f, 0.470588f, 0.541176f);
    }

    glPushMatrix();
    glutSolidSphere(13, 9, 9);
    glPopMatrix();

    glRotatef(-90 + cannonRot[0], 1, 0, 0); // standing cylinder (x-axis)
    glRotatef(cannonRot[1], 0, 1, 0);       // standing cylinder (x-axis)
    glutSolidCylinder(10, 25, 7, 7);
    glPopMatrix();
}

/**
 * Draws everything to the screen in its respective order. The floor, cannon, lights, and then the particles.
 *
 */
void DrawScene()
{

    glPushMatrix();
    // everything is rotated first by the global rotation array, which is controlled with the arrow keys.
    glRotatef(globalRot[0], 1, 0, 0); // x rotation
    glRotatef(globalRot[1], 0, 1, 0); // y rotation
    glRotatef(globalRot[2], 0, 0, 1); // z rotation

    drawFloor();
    drawCannon();
    drawLight();

    for (Particle3D p : particle_list.particles)
    {
        glPushMatrix();
        //apply local translations and rotations
        glTranslatef(p.position.mX, p.position.mY, p.position.mZ); 
        glRotatef(p.rotation.rX, 1, 0, 0);
        glRotatef(p.rotation.rY, 0, 1, 0);
        glRotatef(p.rotation.rZ, 0, 0, 1);

        if (lights_on)
        {
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, p.material.amb);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, p.material.diff);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, p.material.spec);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, p.material.shiny);
        }
        else
        {
            glColor3f(p.colour.r, p.colour.g, p.colour.b);
        }
        if (p.material.type == MaterialType::gold) //gold is rendered as a teapot because why not
        {
            glutSolidTeapot(p.size / 2);
        }
        else if (p.material.type == MaterialType::ruby) //ruby is a sphere but low res
        {
            glutSolidSphere(p.size / 2, 4, 4);
        }
        else //plastic is a smoother sphere
        {
            glutSolidSphere(p.size / 2, 10, 10);
        }

        glPopMatrix();

        //The line trails (very cool)
        glBegin(GL_LINE_STRIP);
        for (Point3D point : p.trail)
        {
            glVertex3f(point.mX, point.mY, point.mZ);
        }
        glEnd();
    }

    glPopMatrix();
}

/**
 * Yeah this is pretty self explanatory I think
 * 
 */
void printInstructions()
{

    const char *instr = "Press the arrow keys to move the selected light.\n"
                        "Press 1 and 2 to change which light is selected\n"
                        "Press L to turn the lights on or off.\n"
                        "Use w a s d to change the direction of the cannon.\n"
                        "Press f to enable friction mode\n"
                        "Press x, y, and z to increase the rotation of the scene in each respective direction.\n"
                        "Press X, Y, and Z to decrease these rotations.\n"
                        "Press p to enter particle camera mode, which follows the next particle until it dies.\n"
                        "Press space to turn the fountain on or off.\n"
                        "Press = and - to zoom in and out respectively.\n"
                        "Press t to add particle trails.\n"
                        "Press r to reset the particles.\n"
                        "Press q to exit the program.";
    std::cout << instr << std::endl;
}

/**
 * Initialize the particle lists, set up the view, etc. 
 * 
 */
void init(void)
{
    origin = Point3D(0, 0, 0);
    particle_list = generateParticles(origin);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    glClearColor(0.2, 0.2, 0.2, 0);
    glColor3f(1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 5000);

    printInstructions();
}

/**
 * Render the scene
 * 
 */
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    //if the particle cam is active, then the camera follows the selected particle
    if (particleCam)
    {
        if (!(&selectedParticleCopy)->shouldDelete())
        {
            Point3D lookAtPoint = (&selectedParticleCopy)->direction.normalize().movePoint((&selectedParticleCopy)->position);
            camPos[0] = (&selectedParticleCopy)->position.mX;
            camPos[1] = (&selectedParticleCopy)->position.mY;
            camPos[2] = (&selectedParticleCopy)->position.mZ;
            lookAt[0] = lookAtPoint.mX;
            lookAt[1] = lookAtPoint.mY;
            lookAt[2] = lookAtPoint.mZ;
        }
        else
        {
            particleCam = false;
        }
    }
    else //else (not particle cam), the camera follows the template position and target
    {
        camPos[0] = CAMPOS[0];
        camPos[1] = CAMPOS[1];
        camPos[2] = CAMPOS[2];
        lookAt[0] = LOOKAT[0];
        lookAt[1] = LOOKAT[1];
        lookAt[2] = LOOKAT[2];
    }

    gluLookAt(camPos[0], camPos[1], camPos[2], lookAt[0], lookAt[1], lookAt[2], 0, 1, 0);


    //Add particles continuously
    if (fountain_on)
    {
        for (int i = 0; i < ParticleList::BURST_SIZE; i++)
        {
            particle_list.addParticle(createParticle(origin));
        }
    }

    DrawScene();

    glutSwapBuffers();
}


/**
 * Keyboard function. Handles all the input specified in the readme
 */
void kbd(unsigned char key, int x, int y)
{
    switch (key)
    {

    case '1':
        selectedLight = 0;
        break;
    case '2':
        selectedLight = 1;
        break;
    case 'f':
    case 'F':
        friction_mode = !friction_mode;
        break;
    case 'x':
        globalRot[0]++;
        break;
    case 'y':
        globalRot[1]++;
        break;
    case 'z':
        globalRot[2]++;
        break;
    case 'X':
        globalRot[0]--;
        break;
    case 'Y':
        globalRot[1]--;
        break;
    case 'Z':
        globalRot[2]--;
        break;
    case 'p':
        if (!particleCam)//you can't do particle cam when already in it
        {

            globalRot[0] = 0;//resets the camera view
            globalRot[1] = 0;
            globalRot[2] = 0;

            selectedParticleCopy = *(particle_list.particles.rbegin());                                    // copy by value
            selectedParticleCopy.position = Vec3D(5.0f, 0, 5.0f).movePoint(selectedParticleCopy.position); // move away slightly so it doesn't clip into the particle
            particleCam = true;
        }
        break;
    case 'r':
    case 'R':
        particle_list.reset();
        break;
    case 'q':
        exit(0);
        break;
    case 32:
        fountain_on = !fountain_on;
        break;
    case 'l':
    case 'L':
        lights_on = !lights_on;
        if (lights_on)
        {
            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);
            glEnable(GL_LIGHT1);
        }
        else
        {
            glDisable(GL_LIGHT0);
            glDisable(GL_LIGHT1);
            glDisable(GL_LIGHTING);
        }
        break;
    case '-':
        CAMPOS[0] = CAMPOS[0] * 1.2;
        CAMPOS[1] = CAMPOS[1] * 1.2;
        CAMPOS[2] = CAMPOS[2] * 1.2;
        break;
    case '=':
        CAMPOS[0] = CAMPOS[0] * 0.8;
        CAMPOS[1] = CAMPOS[1] * 0.8;
        CAMPOS[2] = CAMPOS[2] * 0.8;
        break;
    case 'w':
        if (cannonRot[1] > -45)
            cannonRot[1]--;
        break;
    case 'a':
        if (cannonRot[0] < 45)
            cannonRot[0]++;
        break;
    case 's':
        if (cannonRot[1] < 45)
            cannonRot[1]++;
        break;
    case 'd':
        if (cannonRot[0] > -45)
            cannonRot[0]--;
        break;
    case 't':
        particle_list.trailOn = !particle_list.trailOn;
        break;
    }
}

/**
 * handles the arrow key functionality specifically. Changes the position of the selected ligth source.
 */
void specialkbd(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_DOWN:
        lightPos[selectedLight][0]++;
        break;
    case GLUT_KEY_UP:
        lightPos[selectedLight][0]--;
        break;
    case GLUT_KEY_LEFT:
        lightPos[selectedLight][2]++;
        break;
    case GLUT_KEY_RIGHT:
        lightPos[selectedLight][2]--;
        break;
    }
}

/**
 * Handles things when expanding or shrinking the window
 */
void windowReshapeFunc(int newWidth, int newHeight)
{

    window_size_x = newWidth;
    window_size_y = newHeight;

    glClearColor(0.2, 0.2, 0.2, 0);
    glColor3f(1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 5000);

    glViewport(0, 0, window_size_x, window_size_y);
}

/**
 * Register the glu functions and set up all varibles. Start the main loop. 
 */
int main(int argc, char **argv)
{
    srand(time(NULL));

    glutInit(&argc, argv); // starts up GLUT

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(window_size_x, window_size_y);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("3D Particle Fountain"); // creates the window

    glutDisplayFunc(display); // registers "display" as the display callback function
    glutKeyboardFunc(kbd);
    glutSpecialFunc(specialkbd);
    glutTimerFunc(17, update, 0);
    glutReshapeFunc(windowReshapeFunc);

    glEnable(GL_DEPTH_TEST);
    //clipping for efficiency
    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);

    init();

    glutMainLoop(); // starts the event loop

    return 0;
}