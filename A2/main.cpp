/*
    Thomas Britnell
    400129897
    britnelt
   3GC3 Assignment 2
*/

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include "particle.h"
#include <math.h>
#include <time.h>

//initial window size
int window_size_x = 800;
int window_size_y = 800;

const int INITIAL_POINT_COUNT = 100;

//the minimum speed that a particle can go before stopping
const float MIN_SPEED = 0.005;

Point2D mouse_pos = Point2D(0, 0);

//The following can be adjusted using the keyboard:
float mouse_strength = 0.5;//controls how 'hard' the mouse pushes or pulls on the particles
float friction_coefficient = 0.005;//controls how quickly the particle stop when the mouse isn't pushing them
int range_factor = 5;//global range modifier that controls the range at which particles are affected by the mouse

bool left_mouse;
bool right_mouse;

bool running = true;//used for stopping the movement

std::vector<Particle> particles;

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    for (Particle p : particles)
    {
        glPointSize(p.size);
        glBegin(GL_POINTS);
        glColor3f(p.colour.r, p.colour.g, p.colour.b);
        glVertex2f(p.position.mX, p.position.mY);
        glEnd();
    }

    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
}

/*
Creates a particle at (x,y) with random colour and size. 
If moving == true, gives particle random velocity. Otherwise the particle starts at rest.
*/
void add_point(float x, float y, bool moving = false)
{

    float r = (rand() % 75 + 25) * 0.01;
    float g = (rand() % 75 + 25) * 0.01;
    float b = (rand() % 75 + 25) * 0.01;

    int size = rand() % 20 + 2;
    float x_direction = 0;
    float y_direction = 0;
    float range = rand() % window_size_x / 2 + 25;
    float speed = (rand() % 5 + 1) * 0.25;

    //so you can create a particle that is already moving
    if (moving)
    {
        x_direction = (rand() % 50 - 25);
        y_direction = (rand() % 50 - 25);
    }

    particles.push_back(
        Particle(Point2D(x, y), {r, g, b}, size, Vec2D(x_direction, y_direction), range, speed));
}


/*
Returns an iterator pointing to the particle closest to (x,y) in the global 'particles' vector
*/
std::vector<Particle>::iterator find_closest_particle(float x, float y)
{

    // if (particles.empty())
    // {
    //     throw 505;
    // }
    std::vector<Particle>::iterator nearest;

    float distance = INT_MAX;

    Point2D point = Point2D(x, y);

    for (auto p = (&particles)->begin(); p != (&particles)->end(); ++p)
    {
        if (p->position.fastDistanceTo(point) <= distance)
        {
            distance = p->position.fastDistanceTo(point);
            nearest = p;
        }
    }

    return nearest;
}

/*
Removes the particle from the global 'particles' vector at the iterator specified.
*/
bool remove_point(std::vector<Particle>::iterator particle)
{

    if (particles.size() > 0)
    {
        particles.erase(particle);
        return true;
    }
    else
    {
        return false;
    }
}

/*
Changes the range of every particle either up or down based on delta. 
Positive delta scales the range up, negative scales it down.
*/
void modify_range(int delta)
{
    for (auto p = (&particles)->begin(); p != (&particles)->end(); ++p)
    {
        if (delta < 0)
        {
            p->range = p->range * 0.8;
        }
        else
        {
            p->range = p->range * 1.2;
        }
    }
}

/*
Clears the screen and generates a set amount of random particles.
*/
void generatePoints()
{
    particles.clear();
    for (int i = 0; i < INITIAL_POINT_COUNT; i++)
    {
        float x = rand() % window_size_x + 0;
        float y = rand() % window_size_y + 0;
        add_point(x, y);
    }
}

/*
Returns if a point is within range of another point
*/
bool in_range(Point2D position1, Point2D position2, float range)
{
    return position1.distanceTo(position2) <= range;
}

/*
Called when the current window is reshaped.
*/
void windowReshapeFunc(int newWidth, int newHeight)
{

    window_size_x = newWidth;
    window_size_y = newHeight;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, window_size_x, 0, window_size_y);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, window_size_x, window_size_y);
}

/*
Creates a circle centered around (h,k) with a randomized radius and colour. 
The circle is made up of nearby points, so if there are not enough points nearby, the circle will be incomplete.
*/
void create_circle(float h, float k)
{
    int r = (rand()%200-100) + 100;
    int detail = 10;//controls the amount of points that comprise each half of the circle

    float y;

    auto p = (&particles)->begin();

    Colour c = {(rand() % 75 + 25) * 0.01, (rand() % 75 + 25) * 0.01, (rand() % 75 + 25) * 0.01};

    for (int i = 0; i < 2; i++)//two halves of the circle, so need two loops through the x values
    {
        for (int x = h - r; x < h + r + round(r / detail); x += round(r / detail))
        {

            if (p == (&particles)->end())
            {
                break;
            }

            if (!p->moving_to_target && in_range(p->position, Point2D(h, k), p->range))
            {

                if (i == 0)
                {
                    y = sqrt(pow(r, 2) - pow((x - h), 2)) + k; //top half of circle
                }
                else
                {
                    y = -sqrt(pow(r, 2) - pow((x - h), 2)) + k; //bottom half of circle
                }

                p->target = Point2D(x, y);
                p->moving_to_target = true;
                p->colour = c;
            }
            else
            {
                x -= round(r / detail); //if we didn't find a suitable particle, go back 
            }

            ++p;
        }
    }
}

/*
The timer function that is called 60 times per second.
Handles the calculations for the motion of the particles, including the applied force, friction, and target force. 
Also handles boundary collisions for the particles. 
At each call of the function, each particle's position moves according to the sum of the 'forces' acting on it.
*/
void update_points(int x)
{
    if (running)
    {
        for (auto p = (&particles)->begin(); p != (&particles)->end(); ++p)
        {

            Vec2D applied_force = Vec2D(0, 0);
            Vec2D target_force = Vec2D(0, 0);
            Vec2D acceleration = Vec2D(0, 0);
            Vec2D friction = Vec2D(0, 0);

            //applied force from the mouse
            if (left_mouse && in_range(p->position, mouse_pos, p->range))
            {
                applied_force = Vec2D::createVector(mouse_pos, p->position).normalize().multiply(-mouse_strength);
            }
            else if (right_mouse && in_range(p->position, mouse_pos, p->range))
            {
                applied_force = Vec2D::createVector(mouse_pos, p->position).normalize().multiply(mouse_strength);
            }

            //target force if the particle has a 'target' meaning it is moving towards a specific point
            if (p->moving_to_target)
            {
                target_force = Vec2D::createVector(p->position, p->target).normalize().multiply(mouse_strength);
            }

            //if the particle is moving, add a friction force to stop it
            if ((abs(p->direction.mX) >= MIN_SPEED) && (abs(p->direction.mY) >= MIN_SPEED))
            {
                friction = p->direction.multiply(-friction_coefficient * (p->size));//friction is proportional to the size of the particle
            }
            else
            {
                p->direction = Vec2D(0, 0); //if the speed falls below MIN_SPEED, just stop the particle
            }

            //acceleration is the sum of all forces applying to the particle
            acceleration = Vec2D(applied_force.mX + friction.mX + target_force.mX, applied_force.mY + friction.mY + target_force.mY);
           

            //particle direction (or velocity) changes based on acceleration
            p->direction = Vec2D(p->direction.mX + acceleration.mX, p->direction.mY + acceleration.mY);

            //check and handle boundary collision
            if (p->position.mX >= window_size_x)
            {
                p->direction = Vec2D(p->direction.mX * -1, p->direction.mY);
                p->position = Point2D(window_size_x, p->position.mY);
            }
            else if (p->position.mX <= 0)
            {
                p->direction = Vec2D(p->direction.mX * -1, p->direction.mY);
                p->position = Point2D(0, p->position.mY);
            }

            if (p->position.mY >= window_size_y)
            {
                p->direction = Vec2D(p->direction.mX, p->direction.mY * -1);
                p->position = Point2D(p->position.mX, window_size_y);
            }
            else if (p->position.mY <= 0)
            {
                p->direction = Vec2D(p->direction.mX, p->direction.mY * -1);
                p->position = Point2D(p->position.mX, 0);
            }

            //move the point based on the velocity 
            p->position = p->direction.movePoint(p->position);
        }
    }
    glutTimerFunc(17, update_points, 0);
}

/*
Updates the global mouse position variable
*/
void update_mouse_pos(int x, int y)
{
    mouse_pos.mX = static_cast<float>(x);
    mouse_pos.mY = static_cast<float>(window_size_y - y);
}

/*
Called when the mouse is clicked. 
Used for updating global mouse status variables.
*/
void mouse(int btn, int state, int x, int y)
{
    update_mouse_pos(x, y);

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
        }
    }
    else if (btn == GLUT_RIGHT_BUTTON)
    {
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

/*
Called when the mouse moves. 
*/
void motion(int x, int y)
{
    update_mouse_pos(x, y);
}

/*
Keyboard function. 
Handles all of the keyboard input.
*/
void kbd(unsigned char key, int x, int y)
{

    std::vector<Particle>::iterator p;
    Point2D location;
    int size;

    switch (key)
    {
    case 'a':
        std::cout << "add particle at:" << x << ", " << y << std::endl;
        add_point(static_cast<float>(x), static_cast<float>(window_size_y - y));

        break;
    case 'c':
        std::cout << "make a circle!" << std::endl;
        create_circle(static_cast<float>(x), static_cast<float>(window_size_y - y));
        break;
    case 'd':
        std::cout << "remove particle" << std::endl;
        if (remove_point(find_closest_particle(static_cast<float>(x), static_cast<float>(window_size_y - y))))
        {
            std::cout << "success" << std::endl;
        }
        else
        {
            std::cout << "No particles to erase!" << std::endl;
        }
        break;

    case 'e':
        std::cout << "explode particle" << std::endl;

        if (particles.size()>0){
            
            p = find_closest_particle(static_cast<float>(x), static_cast<float>(window_size_y - y));
            location = p->position;
            size = p->size;

            if (remove_point(p))
            {
                for (int i = 0; i < size - 1; i++)
                {
                    add_point(location.mX, location.mY, true);
                }
            }
        }
        else
        {
            std::cout << "No particles to explode!" << std::endl;
        }
        break;
    case '+':
        std::cout << "increase range" << std::endl;
        if (range_factor < 10)
        {
            modify_range(1);
            range_factor += 1;
        }
        else
        {
            std::cout << "max range" << std::endl;
        }

        break;
    case '-':
        std::cout << "decrease range" << std::endl;
        if (range_factor > 0)
        {
            modify_range(-1);
            range_factor -= 1;
        }
        else
        {
            std::cout << "min range" << std::endl;
        }

        break;
    case ']':
        std::cout << "increase speed" << std::endl;
        if (mouse_strength < 1)
        {

            mouse_strength += 0.1;
        }
        else
        {
            std::cout << "max speed" << std::endl;
        }
        break;
    case '[':
        std::cout << "decrease speed" << std::endl;
        if (mouse_strength > 0.1)
        {

            mouse_strength -= 0.1;
        }
        else
        {
            std::cout << "min speed" << std::endl;
        }
        break;
    case 'p':
        std::cout << "increase friction" << std::endl;
        if (friction_coefficient < 0.01)
        {

            friction_coefficient += 0.001;
        }
        else
        {
            std::cout << "max friction" << std::endl;
        }
        break;
    case 'o':
        std::cout << "decrease friction" << std::endl;
        if (friction_coefficient > 0.001)
        {

            friction_coefficient -= 0.001;
        }
        else
        {
            std::cout << "min friction" << std::endl;
        }
        break;
    case 32:
        std::cout << "Pause simulation" << std::endl;
        running = !running;
        break;
    case 'r':
        std::cout << "Reset scene" << std::endl;
        generatePoints();
        break;
    case 'q':
        std::cout << "quit the program" << std::endl;
        exit(0);
        break;
    }
}

void print_info()
{
    std::cout << "---PROGRAM USAGE---\n\n"
              << "a : add a particle at the cursor\n"
              << "d : delete particle closest to the cursor\n"
              << "c : create a circle at the cursor with random radius from nearby particles (wont make a full circle if there aren't enough nearby particles! I did this on purpose)\n"
              << "e : explode the closest particle\n"
              << "+ : increase particle range\n"
              << "- : decrease particle range\n"
              << "] : increase particle speed\n"
              << "[ : decrease particle speed\n"
              << "p : increase friction\n"
              << "o : decrease friction\n"
              << "space : pause simulation\n"
              << "r : reset scene\n"
              << "q : quit the program" << std::endl;
}

/* main function - program entry point */
int main(int argc, char **argv)
{
    print_info();
    srand(time(NULL));
    generatePoints();
    glutInit(&argc, argv); //starts up GLUT
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(window_size_x, window_size_y);
    glutCreateWindow("Particle Simulation.exe"); //creates the window
                                                 // gluOrtho2D(0, window_size_x, 0, window_size_y);
    glutTimerFunc(17, update_points, 0);
    glutKeyboardFunc(kbd);
    glutMotionFunc(motion);
    glutDisplayFunc(display);
    glutReshapeFunc(windowReshapeFunc);
    glutMouseFunc(mouse);
    glutMainLoop();
    return (0);
}
