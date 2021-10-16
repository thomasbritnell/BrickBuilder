#include "headers/particleList.h"
#include <random>
#include <time.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

float pos[] = {0,1,0};
float rot[] = {0, 0, 0};
float headRot[] = {0, 0, 0};
float camPos[] = {100, 250, 100};

bool fountain_on = true;

Point3D origin;
ParticleList particle_list;


void drawAxis(){
    glPushMatrix();
    glLineWidth(2);
    glBegin(GL_LINES);

    glColor3f (1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(5.0, 0.0, 0.0);

    glColor3f (1.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 5.0, 0.0);

    glColor3f (0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 5.0);
    glEnd();
    glPopMatrix();
}

void drawFloor(){

    float size = 100.0f;
    glPushMatrix();

    glBegin(GL_QUADS);
        glColor3f(0.1f,0.2f,0.25f);
        glVertex3f(-size,0.0f,0.0f);
        glVertex3f(0.0f,0.0f,size);
        glVertex3f(size,0.0f,0.0f);
        glVertex3f(0.0f,0.0f,-size);
    glEnd();

    glPopMatrix();
}

void update(int unused){

    particle_list.updateParticles();

    glutPostRedisplay();
    glutTimerFunc(17,update,0);
}


float random(int min, int max){
    return rand() % (max-min) + min;
}


Particle3D createParticle(Point3D origin){
    Point3D position = origin;
    Vec3D direction = Vec3D(random(-100,100)*0.001,1,random(-100,100)*0.001);
    int speed = random(3,8);
    Angle rotation = {0.0f,0.0f,0.0f};
    int size = random(1,5);
    Colour colour = {random(0,50)*0.01,random(0,50)*0.01,random(50,100)*0.01};
    Material material = static_cast<Material>(random(0,3));
    int age = 0;

    return Particle3D(position,direction,speed,rotation,size,colour,material,age);
}

ParticleList generateParticles(Point3D origin){

    std::vector<Particle3D> particles;

    for(int i = 0; i < ParticleList::BURST_SIZE ; i++){
        
        particles.push_back(createParticle(origin));
    }
    ParticleList list = ParticleList(particles,origin);
    return list;
}


void DrawSnowman(float* pos, float* rot)
{
	glPushMatrix();
	drawFloor();

    glRotatef(rot[1], 0, 1, 0);
	glTranslatef(pos[0], pos[1], pos[2]);

	
	//drawAxis();
	//draw body
	glColor3f(1,1,1);
	glutSolidSphere(1, 16, 16);

	//draw buttons
	glPushMatrix();
	glTranslatef(0, 0.35, 0.9);
	glScalef(0.1,0.1,0.1);
	//drawAxis();
	glColor3f(0, 0, 0);
	glutSolidSphere(1, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.15, 0.95);
	glScalef(0.1,0.1,0.1);
	//drawAxis();
	glColor3f(0, 0, 0);
	glutSolidSphere(1, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.05, 0.95);
	glScalef(0.1,0.1,0.1);
	//drawAxis();
	glColor3f(0, 0, 0);
	glutSolidSphere(1, 10, 10);
	glPopMatrix();


	glPushMatrix();
	//translate relative to body, and draw head
	glTranslatef(0, 1.25, 0);
	glRotatef(headRot[1], 0, 1, 0); //turn the head relative to the body
	glColor3f(1,1,1);
	glutSolidSphere(0.5, 16, 16);
	
	//translate and draw right eye
	glPushMatrix();
	glTranslatef(0.2, 0.15, 0.45);
	glColor3f(0,0,0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	//translate and draw left eye
	glPushMatrix();
	glTranslatef(-0.2, 0.15, 0.45);
	glColor3f(0,0,0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	//translate and draw nose
	glPushMatrix();
	glTranslatef(0, 0, 0.5);
	glColor3f(1,0.4,0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	glPopMatrix();//body
	glPopMatrix();//snowman

	//glutSolidSphere(1,10,10);
}

void DrawParticles(){
    drawFloor();
    for (Particle3D p : particle_list.particles){
        glPushMatrix();
        glTranslatef(p.position.mX,p.position.mY,p.position.mZ);
        //glRotatef(p.rotation.rX,p.rotation.rY,p.rotation.rZ);
        glColor3f(p.colour.r,p.colour.g,p.colour.b);
        glutSolidSphere(p.size/2,10,10);
        glPopMatrix();
    }
}

void init(void)
{
    origin = Point3D(0,0,0);
    particle_list = generateParticles(origin);



	glClearColor(0.5, 0.5, 0.5, 0);
	glColor3f(1, 1, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-2, 2, -2, 2, -2, 2);
	gluPerspective(45, 1, 1, 500);
}

void display(void)
{
	//float origin[3] = {0,0,0};
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(camPos[0], camPos[1], camPos[2], 0,0,0, 0,100,0);

	//DrawSnowman(pos, rot);
    DrawParticles();

    for(int i = 0;i < ParticleList::BURST_SIZE ; i++){
            particle_list.addParticle(createParticle(origin));
        }


	glutSwapBuffers();
}

void kbd(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'x':
        camPos[0]++;
        break;
    case 'y':
        camPos[1]++;
        break;
    case 'z':
        camPos[2]++;
        break;
    case 'X':
        camPos[0]--;
        break;
    case 'Y':
        camPos[1]--;
        break;
    case 'Z':
        camPos[2]--;
        break;

    case 'r':
    case 'R':
        particle_list = generateParticles(origin);
        break;
    case 'a':
        for(int i = 0;i < ParticleList::BURST_SIZE ; i++){
            particle_list.addParticle(createParticle(origin));
        }
        
        break;
    }
}

int main(int argc, char **argv){
    srand(time(NULL));
    

    glutInit(&argc, argv);		//starts up GLUT
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    
	
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("3D Particle Fountain");	//creates the window

	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(kbd);
    glutTimerFunc(17,update,0);


	glEnable(GL_DEPTH_TEST);
    
	glFrontFace(GL_CW);
	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);
    
	init();

	glutMainLoop();				//starts the event loop


    return 0;
}