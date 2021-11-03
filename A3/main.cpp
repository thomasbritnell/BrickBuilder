#include "headers/particleList.h"
#include <random>
#include <time.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <iostream>


float globalRot[] = {0,0,0};

int CAMPOS[3] = {300, 300, 300};
float camPos[3] = {static_cast<float>(CAMPOS[0]),static_cast<float>(CAMPOS[1]),static_cast<float>(CAMPOS[2])};
float lookAt[3];
const int LOOKAT[3] = {0,0,0};
float lightPos[] = {0,100,0,1};

bool fountain_on = true;
bool friction_mode = true;

bool lights_on = true;
bool particleCam = false;
std::vector<Particle3D>::iterator particlePerspective;

float pos[4] = {2, 2, 2, 1}; 
float amb[4] = {1, 1, 1, 1}; 
float dif[4] = {1, 1, 1, 1}; 
float spc[4] = {1, 1, 1, 1}; 

Point3D origin;
ParticleList particle_list;


void drawAxis(){
    glPushMatrix();
    // glLineWidth(2);
    // glBegin(GL_LINES);

    // glColor3f (1.0, 0.0, 0.0);
    // glVertex3f(0.0, 0.0, 0.0);
    // glVertex3f(5.0, 0.0, 0.0);

    // glColor3f (1.0, 1.0, 0.0);
    // glVertex3f(0.0, 0.0, 0.0);
    // glVertex3f(0.0, 5.0, 0.0);

    // glColor3f (0.0, 0.0, 1.0);
    // glVertex3f(0.0, 0.0, 0.0);
    // glVertex3f(0.0, 0.0, 5.0);
    // glEnd();
    glutSolidTeapot(10);
    glPopMatrix();
}

void drawLight(){
    glPushMatrix();
    //glLoadIdentity();

    glTranslatef(lightPos[0],lightPos[1],lightPos[2]);
    glutSolidTeapot(10);

    glPopMatrix();
}

void drawFloor(){

    float size = 200.0f;
    glPushMatrix();
    if (lights_on){
            //Obsidian
            float amb[] = { 0.05375f, 0.05f, 0.06625f, 0.82f };
            float diff[] = { 0.18275f, 0.17f, 0.22525f, 0.82f};
            float spec[] = {0.332741f, 0.328634f, 0.346435f, 0.82f };
            float shin = 38.4f ;

            glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,amb);
            glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diff);
            glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,spec);
            glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,&shin);
            
        }else{
            glColor3f(0.1f,0.2f,0.25f);
        }
    glBegin(GL_QUADS);
        
        
        glVertex3f(-size,-10.0f,-size);
        glVertex3f(-size,-10.0f,size);
        glVertex3f(size,-10.0f,size);
        glVertex3f(size,-10.0f,-size);
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
    Vec3D direction = Vec3D(random(-100,100)*particle_list.direction.mX,particle_list.direction.mY,random(-100,100)*particle_list.direction.mZ);
    int speed = random(3,8);
    Angle rotation = {0.0f,0.0f,0.0f};
    int size = random(4,7);
    Colour colour = {random(0,100)*0.01f,random(0,100)*0.01f,random(0,100)*0.01f};


    MaterialType m = static_cast<MaterialType>(random(0,3));
    //std::cout<< m << std::endl;

    Material material = Material(m);
    int age = 0;

    return Particle3D(position,direction,speed,rotation,size,colour,material,age);
}

ParticleList generateParticles(Point3D origin){

    std::vector<Particle3D> particles;

    for(int i = 0; i < ParticleList::BURST_SIZE ; i++){
        
        Particle3D new_particle = createParticle(origin);

        particles.push_back(new_particle);
    }
    ParticleList list = ParticleList(particles,origin);
    return list;
}

void DrawScene(){

    glPushMatrix();
    glRotatef(globalRot[0],1,0,0);//x rotation
    glRotatef(globalRot[1],0,1,0);//y rotation
    glRotatef(globalRot[2],0,0,1);//z rotation




    drawAxis();
    drawFloor();
    drawLight();

    for (Particle3D p : particle_list.particles){
        glPushMatrix();
        glTranslatef(p.position.mX,p.position.mY,p.position.mZ);
        //glRotatef(p.rotation.rX,p.rotation.rY,p.rotation.rZ);

    
        if (lights_on){
            glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,p.material.amb);
            glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,p.material.diff);
            glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,p.material.spec);
            glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,p.material.shiny);
        }else{
            glColor3f(p.colour.r,p.colour.g,p.colour.b);
        }
        
        glutSolidSphere(p.size/2,7,7);
        glPopMatrix();
    }

    glPopMatrix();
}

// void particleCam(Particle3D particle){
//     // camPos = {};
//     while (particle.markForDeletion){
//         Point3D lookAtPoint = particle.direction.normalize().movePoint(particle.position);
//         std::cout << lookAtPoint.mX << lookAtPoint.mY << lookAtPoint.mZ <<std::endl;
//         // gluLookAt(particle.position.mX, particle.position.mY, particle.position.mZ, lookAt.mX, lookAt.mY, lookAt.mZ, 0,1,0);
//         camPos[0] = particle.position.mX;
//         camPos[1] = particle.position.mY;
//         camPos[2] = particle.position.mZ;
//         lookAt[0] = lookAtPoint.mX;
//         lookAt[1] = lookAtPoint.mY;
//         lookAt[2] = lookAtPoint.mZ;
//     }
    

// }

void init(void)
{
    origin = Point3D(0,0,0);
    particle_list = generateParticles(origin);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// glOrtho(-250, 250, -250, 250, -500, 500);
	gluPerspective(45, 1, 1, 1000);
}

void display(void)
{
	//float origin[3] = {0,0,0};
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    if (particleCam)
    {
        if (!particlePerspective->shouldDelete()){
            Point3D lookAtPoint = particlePerspective->direction.normalize().movePoint(particlePerspective->position);
            camPos[0] = particlePerspective->position.mX;
            camPos[1] = particlePerspective->position.mY;
            camPos[2] = particlePerspective->position.mZ;
            lookAt[0] = lookAtPoint.mX;
            lookAt[1] = lookAtPoint.mY;
            lookAt[2] = lookAtPoint.mZ;
        }        
        
    }else{
        particleCam = false;
        camPos[0] = CAMPOS[0];
            camPos[1] = CAMPOS[1];
            camPos[2] = CAMPOS[2];
            lookAt[0] = LOOKAT[0];
            lookAt[1] = LOOKAT[1];
            lookAt[2] = LOOKAT[2];
    }

    gluLookAt(camPos[0], camPos[1], camPos[2], lookAt[0], lookAt[1], lookAt[2], 0,1,0);
	  
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos); 
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dif); 
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb); 
    glLightfv(GL_LIGHT0, GL_SPECULAR, spc);

    
    if (fountain_on){
        for(int i = 0;i < ParticleList::BURST_SIZE ; i++){
            particle_list.addParticle(createParticle(origin));
        }
    }
    
    DrawScene();


	glutSwapBuffers();
}

void kbd(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'x':
        //camPos[0]++;
        globalRot[0]++;
        break;
    case 'y':
        //camPos[1]++;
        globalRot[1]++;
        break;
    case 'z':
        //camPos[2]++;
        globalRot[2]++;
        break;
    case 'X':
        //camPos[0]--;
        globalRot[0]--;
        break;
    case 'Y':
        //camPos[1]--;
        globalRot[1]--;
        break;
    case 'Z':
        //camPos[2]--;
        globalRot[2]--;
        break;
    case 'p':
        if(!particleCam){
            std::vector<Particle3D>::iterator selectedParticle = std::prev((&(particle_list.particles))->end());
            particlePerspective = selectedParticle;
            particleCam = true;
        }
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
    case 'q':
    exit(0);
    break;
    case 32:
        fountain_on = !fountain_on;
    break;
    case 'l':
    case 'L':
        lights_on = !lights_on;
    break;
    case '=':
        CAMPOS[0] = CAMPOS[0]*1.2;
        CAMPOS[1] = CAMPOS[1]*1.2;
        CAMPOS[2] = CAMPOS[2]*1.2;
    break;
    case '-':
        CAMPOS[0] = CAMPOS[0]*0.8;
        CAMPOS[1] = CAMPOS[1]*0.8;
        CAMPOS[2] = CAMPOS[2]*0.8;
    break;

    }
}
void specialkbd(int key, int x, int y){
    switch(key){
        case GLUT_KEY_DOWN:
            lightPos[0]--;
        break;
        case GLUT_KEY_UP:
            lightPos[0]++; 
        break;
        case GLUT_KEY_LEFT:
            lightPos[2]--; 
        break;
        case GLUT_KEY_RIGHT: 
            lightPos[2]++;
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
    glutSpecialFunc(specialkbd);
    glutTimerFunc(17,update,0);


	glEnable(GL_DEPTH_TEST);
    
	glFrontFace(GL_CW);
	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);
    
	init();

	glutMainLoop();				//starts the event loop


    return 0;
}