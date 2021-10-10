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
#include <math.h>
#include <time.h>

//initial window size
int window_size_x = 600;
int window_size_y = 600;

const int INITIAL_POINT_COUNT = 500;

//the minimum speed that a particle can go before stopping
const float MIN_SPEED = 0.005;

Point2D mouse_pos = Point2D(0,0);

float mouse_strength = 0.5;
float friction_coefficient = 0.005;
int range_factor = 5;


bool left_mouse;
bool right_mouse;


bool running = true;

std::vector<Particle> particles;


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    for (Particle p : particles)
    {
        glPointSize(p.size);
        glBegin(GL_POINTS);
        glColor3f(p.colour.r,p.colour.g,p.colour.b);
        glVertex2f(p.position.mX, p.position.mY);
        glEnd();
    }

    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
}

void add_point(float x, float y, bool moving = false){
        
       
        float r = (rand() % 75 + 25)*0.01;
        float g = (rand() % 75 + 25)*0.01;
        float b = (rand() % 75 + 25)*0.01;
        
        int size = rand() % 20 + 2;
        float x_direction = 0 ; 
        float y_direction = 0 ; 
        float range = rand() % window_size_x/2 + 10;
        float speed = (rand() % 5 + 1)*0.25;

        if (moving){
            x_direction = (rand() % 50 - 25);
            y_direction = (rand() % 50 - 25);
        }

        particles.push_back(
            Particle(Point2D(x,y),{r,g,b},size,Vec2D(x_direction,y_direction),range,speed)
        );
}

std::vector<Particle>::iterator find_closest_particle(float x, float y){

    if (particles.empty()){
        throw 505;
    }
    std::vector<Particle>::iterator nearest;

    float distance = INT_MAX;
    
    Point2D point = Point2D(x,y);

   for(auto p = (&particles)->begin(); p != (&particles)->end(); ++p)
        {
            if (p->position.fastDistanceTo(point) <= distance){
                distance = p->position.fastDistanceTo(point);
                nearest = p;
            }
        }
    
    return nearest;
}

bool remove_point(std::vector<Particle>::iterator particle){
   
   
    if (particles.size() > 0){   
        particles.erase(particle);
        return true;
    }else{
        return false;
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
        float x = rand() % window_size_x + 0;
        float y = rand() % window_size_y + 0;
        add_point(x,y);
    }
}

bool in_range(Point2D position1,Point2D position2,float range){
    return position1.distanceTo(position2) <= range;
}

void windowReshapeFunc( int newWidth, int newHeight ) {
    

    window_size_x = newWidth;
    window_size_y = newHeight;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, window_size_x, 0, window_size_y);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0,0,window_size_x,window_size_y);
  

}

void create_circle(float h, float k){
    int r = 100;
    int detail = 10;

    float y;

    auto p = (&particles)->begin();

    Colour c = {(rand() % 75 + 25)*0.01,(rand() % 75 + 25)*0.01,(rand() % 75 + 25)*0.01};

   // k = window_size_y - k;
   for (int i = 0; i< 2;i++){
       for (int x = h-r; x < h+r + round(r/detail); x += round(r/detail) ){
        
        if (p == (&particles)->end()){break;}

        if (!p->moving_to_target && in_range(p->position,Point2D(h,k),p->range)){

            if(i==0){
                y = sqrt(pow(r,2)-pow((x-h),2)) + k;
            }else{
                y = -sqrt(pow(r,2)-pow((x-h),2)) + k;
            }

            std::cout<<"circle point"<<x<<", "<<y<<std::endl;

            p->target = Point2D(x,y);
            p->moving_to_target = true;
            p->colour = c;
        }else{
            x -= round(r/detail);
        }

        ++p;

    }
   }
}

void update_points(int x){
    if (running){
        for(auto p = (&particles)->begin(); p != (&particles)->end(); ++p)
        {

            Vec2D applied_force = Vec2D(0,0);
            Vec2D target_force = Vec2D(0,0);
            Vec2D acceleration = Vec2D(0,0);
            Vec2D friction = Vec2D(0,0);

            if (left_mouse && in_range(p->position,mouse_pos,p->range)){
                applied_force = Vec2D::createVector(mouse_pos,p->position).normalize().multiply(-mouse_strength);
            }else if(right_mouse && in_range(p->position, mouse_pos,p->range)){
                applied_force = Vec2D::createVector(mouse_pos,p->position).normalize().multiply(mouse_strength);
            }

            if (p->moving_to_target){
                target_force = Vec2D::createVector(p->position,p->target).normalize().multiply(mouse_strength);
            }
            

            if ((abs(p->direction.mX) >= MIN_SPEED) && (abs(p->direction.mY) >= MIN_SPEED)){
                friction = p->direction.multiply(-friction_coefficient*(p->size));
            }else{
                p->direction = Vec2D(0,0);
            }

            acceleration = Vec2D(applied_force.mX+friction.mX+target_force.mX, applied_force.mY+friction.mY+target_force.mY);
            //std::cout<< "acceleration: " <<acceleration.mX << ", " << acceleration.mY << "\n";

            //janky 'vector add' function because i dont want to modify Vec2D itself
            p->direction = Vec2D(p->direction.mX + acceleration.mX, p->direction.mY + acceleration.mY);
            //std::cout<< "velocity: " << p->direction.mX << ", " << p->direction.mY << "\n";


            //check and handle boundary collision
            if (p->position.mX >= window_size_x){
                p->direction = Vec2D(p->direction.mX*-1,p->direction.mY);
                p->position = Point2D(window_size_x ,p->position.mY);
            }else if (p->position.mX <= 0){
                p->direction = Vec2D(p->direction.mX*-1,p->direction.mY);
                p->position = Point2D(0 ,p->position.mY);
            }

            if (p->position.mY >= window_size_y){
                p->direction = Vec2D(p->direction.mX,p->direction.mY*-1);
                p->position = Point2D(p->position.mX,window_size_y );
            }else if (p->position.mY <= 0){
                p->direction = Vec2D(p->direction.mX,p->direction.mY*-1);
                p->position = Point2D(p->position.mX,0 );
            }

        

            p->position = p->direction.movePoint(p->position);


            
            // for (auto other = (&particles)->begin(); other != (&particles)->end(); ++other){
            //     if (p != other && p->collideWith(*other)){
            //         //std::cout<<"collide!\n";
            //         //p->direction = Vec2D((other->size/p->size)*p->direction.mX+(other->direction.mX),(other->size/p->size)*p->direction.mY+(other->direction.mY));
            //         //add_point((p->position.mX+other->position.mX)/2,(p->position.mY+other->position.mY)/2,false,true,p->colour.r,p->colour.g,p->colour.b);
            //         //remove_point(p);
            //         //remove_point(other);
            //     }
            // }
        }
    }
    //glutPostRedisplay();
    glutTimerFunc(17,update_points,0);
}

void update_mouse_pos(int x, int y){
    mouse_pos.mX = static_cast<float>(x);
    mouse_pos.mY = static_cast<float>(window_size_y - y);
}

void mouse(int btn, int state, int x, int y)
{
    update_mouse_pos(x,y);

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
    update_mouse_pos(x,y);
}



void kbd(unsigned char key, int x, int y)
{

    std::vector<Particle>::iterator p;
    Point2D location;
    int size;

    switch (key)
    {
        //increases the size of the points only if it is less than the max
    case 'a':
            std::cout << "add particle at:" << x <<", "<<y << std::endl; 
            add_point(static_cast<float>(x),static_cast<float>(window_size_y - y));

        break;
    case 'c':
            std::cout<<"make a circle!"<<std::endl;
            create_circle(static_cast<float>(x),static_cast<float>(window_size_y - y));
        break;
    case 'd':
            std::cout << "remove particle" << std::endl; 
            if (remove_point(find_closest_particle(static_cast<float>(x),static_cast<float>(window_size_y - y)))){
                std::cout << "success" <<std::endl;
            }else{
                std::cout << "No particles to erase!"<< std::endl;
            }
        break;

    case 'e':
             std::cout << "explode particle" << std::endl; 

             p = find_closest_particle(static_cast<float>(x),static_cast<float>(window_size_y - y));
             location = p->position;
             size = p->size;

             if (remove_point(p)){
                 for (int i = 0; i < size-1; i++){
                     add_point(location.mX,location.mY,true);
                 }
             }else{
                 std::cout << "No particles to explode!"<< std::endl;
             }

            // Particle p = remove_point(static_cast<float>(x),static_cast<float>(window_size_y - y));
            // if (p != NULL){
            //     for (int i = 0; i < p.size; i++){
            //         add_point(p.position.mX,p.position.mY,true);
            //     }
            // }else{
            //     std::cout << "No particles to explode!"<< std::endl;
            // };
            
        break;
    case '+':
            std::cout << "increase range"<< std::endl;
            if (range_factor < 10){
                modify_range(1);
                range_factor += 1;
            }else{
                std::cout << "max range"<< std::endl;
            }
        
        break;
    case '-':
            std::cout << "decrease range"<< std::endl;
            if (range_factor > 0){
                modify_range(-1);
                range_factor -= 1;
            }else{
                std::cout << "min range"<< std::endl;
            }
        
        break;
    case ']':
            std::cout << "increase speed" << std::endl;
            if (mouse_strength < 1){
                
                mouse_strength += 0.1;
            }else{
                std::cout << "max speed" << std::endl;
            }
        break;
    case '[':
            std::cout << "decrease speed"<< std::endl;
            if (mouse_strength > 0.1){
                
                mouse_strength -= 0.1;
            }else{
                std::cout << "min speed"<< std::endl;
            }
        break;
    case 'p':
            std::cout << "increase friction"<< std::endl;
            if (friction_coefficient < 0.01){
                
                friction_coefficient += 0.001;
            }else{
                std::cout << "max friction"<< std::endl;
            }
        break;
    case 'o':
            std::cout << "decrease friction"<< std::endl;
            if (friction_coefficient > 0.001){
                
                friction_coefficient -= 0.001;
            }else{
                std::cout << "min friction"<< std::endl;
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


void print_info(){
    std::cout << "---PROGRAM USAGE---\n\n" << 
    "a : add a particle at the cursor\n" << 
    "d : delete particle closest to the cursor\n" <<
    "c : create a circle at the cursor\n" <<
    "e : explode the closest particle\n" <<
    "+ : increase particle range\n" << 
    "- : decrease particle range\n" <<
    "] : increase particle speed\n" <<
    "[ : decrease particle speed\n" << 
     "p : increase friction\n" <<
    "o : decrease friction\n" <<
    "space : pause simulation\n" <<
    "r : reset scene\n" <<
    "q : quit the program" << std::endl;
}

/* main function - program entry point */
int main(int argc, char **argv)
{
    print_info();
    srand (time(NULL));
    generatePoints();
    glutInit(&argc, argv); //starts up GLUT
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(window_size_x, window_size_y);
    glutCreateWindow("Particle Simulation.exe"); //creates the window
   // gluOrtho2D(0, window_size_x, 0, window_size_y);
    glutTimerFunc(17,update_points,0);
    glutKeyboardFunc(kbd);
    glutMotionFunc(motion);
    glutDisplayFunc(display);
    glutReshapeFunc(windowReshapeFunc);
    glutMouseFunc(mouse);
    glutMainLoop();
    return (0);
}
