#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <iostream>
#include <vector>
#include <string>

#include "header/camera.h"
#include "header/fileHandler.h"
#include "header/object.h"
// #include "texturedCube.cpp"

#include "PPM.h"

int window_size_x = 800;
int window_size_y = 800;

Point3D camPos = Point3D(100,100,100);
Point3D camTarget = Point3D(0,0,0);
Vec3D camUp = Vec3D(0,1,0);
Camera camera;

bool alt; //control pressed
bool shift;
bool alt_shift; //both control and shift pressed

Point3D initialRightClickPosition;
float previousRightClickPosition[2];

bool leftClick;
bool rightClick;

float selectedLight = 0;
bool lights_on = true;
float lightPos[2][4] = {{0,50,-50,1},{0,50,50,1}};
float pos[4] = {2, 2, 2, 1}; 
float amb[4] = {1, 1, 1, 1}; 
float dif[4] = {1, 1, 1, 1}; 
float spc[4] = {1, 1, 1, 1}; 

Material selectedMaterial;


std::vector<Object> objects;

Object* selectedObject;

Vec3D cursorRay = Vec3D();
Plane ground = Plane(Point3D(0,0,0),Vec3D(0,1,0));

GLdouble start[3] = {0,0,0};
GLdouble end[3] = {0,0,0};

struct Image {
    int mWidth;
    int mHeight;
    GLubyte * mImage;

    void load(char * filename) {
        mImage = LoadPPM(filename, &mWidth, &mHeight);
    }

    void draw(unsigned int x, unsigned int y) {
        glRasterPos2i(x + mWidth, y);
        glPixelZoom(-1, 1);
        glDrawPixels(mWidth, mHeight, GL_RGB, GL_UNSIGNED_BYTE, mImage);

    }

    void texture() {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, mImage);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
};

Image selectedTexture;
Image marbleTexture;
Image floorTexture;
Image brickTexture;


Point3D rayPlaneIntersection(Vec3D ray, Plane plane){
    float D = plane.position.distanceTo(Point3D()); 
    float vectorsDotProd = Vec3D::dotProduct(plane.normal,ray);
    // if(vectorsDotProd == 0){ // ray is 90deg to plane
    //     return Point3D(); // for now, must change to a null or some way to say it didnt work
    // }

    // std::cout << "draw Donut" << vectorsDotProd << std::endl;
    float t = (-(Vec3D::dotProduct(plane.normal,ray.start) + D)) / vectorsDotProd;

    //std::cout << "draw Donut" << ray.start.mY << std::endl;

    float pX = ray.start.mX + t * ray.mX;
    float pY = ray.start.mY + t * ray.mY;
    float pZ = ray.start.mZ + t * ray.mZ;

    return Point3D(pX,pY,pZ);
}

void update(int unused){


    glutPostRedisplay();
    glutTimerFunc(17,update,0);
}

void resetScene(){
    objects.clear();
    lightPos[0][0] = 0;
    lightPos[0][1] = 50;
    lightPos[0][2] = -50;
    lightPos[1][0] = 0;
    lightPos[1][1] = 50;
    lightPos[1][2] = 50;
    selectedObject = nullptr;

}

void loadScene(float* data){
     

    //light positions
    lightPos[0][0] = data[1];
    lightPos[0][1] = data[2];
    lightPos[0][2] = data[3];
    lightPos[1][0] = data[4];
    lightPos[1][1] = data[5];
    lightPos[1][2] = data[6];

    //objects!

    objects.clear();


    for (int i = 0 ; i < data[0]; i++){
        int j = i*11+7;

        std::cout << "loading object:" << std::endl;
        std::cout<<"position:\n" << data[j] << ", " << data[j+1] << ", " << data[j+2] << std::endl;
        
        std::cout<<"rotation:\n" << data[j+3] << ", " << data[j+4] << ", " << data[j+5] << std::endl;
        
        std::cout<<"scale:\n" << data[j+6] << ", " << data[j+7] << ", " << data[j+8] << std::endl;

        Object temp = Object(Material(static_cast<MaterialType>(data[j+9])),static_cast<ObjectType>(data[j+10]));
        temp.position = Point3D(data[j],data[j+1],data[j+2]);
        temp.rotation = Point3D(data[j+3],data[j+4],data[j+5]);
        temp.scale = Point3D(data[j+6],data[j+7],data[j+8]);
        objects.push_back(temp);
    }

}

void drawLight(){
    glPushMatrix();
    //glLoadIdentity();
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos[0]); 
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dif); 
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb); 
    glLightfv(GL_LIGHT0, GL_SPECULAR, spc);

    
    glTranslatef(lightPos[0][0],lightPos[0][1],lightPos[0][2]);
    glutSolidSphere(3,16,16);

    glPopMatrix();

    glPushMatrix();
    //glLoadIdentity();
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos[1]); 
    glLightfv(GL_LIGHT1, GL_DIFFUSE, dif); 
    glLightfv(GL_LIGHT1, GL_AMBIENT, amb); 
    glLightfv(GL_LIGHT1, GL_SPECULAR, spc);

    
    glTranslatef(lightPos[1][0],lightPos[1][1],lightPos[1][2]);
    glutSolidSphere(3,16,16);

    glPopMatrix();
}

void drawAxis(){
    float size = 100.0f;

    glPushMatrix();
    glLineWidth(2);
    glBegin(GL_LINES);

     if (lights_on){
        // red rubber
        float amb[] ={ 0.05f,0.0f,0.0f,1.0f };
        float diff[] ={ 0.5f,0.4f,0.4f,1.0f};
        float spec[] ={0.7f,0.04f,0.04f,1.0f };
        float shine =  10.0f;

        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,amb);
        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diff);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,spec);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,&shine);
            
    }
    else{
        glColor3f(1.0,0.0,0.0);
    }
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(size, 0.0, 0.0);

    if (lights_on){
        // yellow rubber
        float amb[] ={ 0.05f,0.05f,0.0f,1.0f };
        float diff[] ={ 0.5f,0.5f,0.4f,1.0f};
        float spec[] ={0.7f,0.7f,0.04f,1.0f };
        float shine =  10.0f;

        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,amb);
        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diff);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,spec);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,&shine);
            
    }
    else{
        glColor3f(1.0,1.0,0.0);
    }
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, size, 0.0);

    if (lights_on){
        // red rubber
        float amb[] ={ 0.0f,0.0f,0.05f,1.0f };
        float diff[] ={ 0.4f,0.4f,0.5f,1.0f};
        float spec[] ={0.04f,0.04f,0.7f,1.0f };
        float shine =  10.0f;

        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,amb);
        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diff);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,spec);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,&shine);
            
    }
    else{
        glColor3f(0.0,0.0,1.0);
    }
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, size);
    glEnd();
    glPopMatrix();
}


void drawFloor(){

    float size = 100.0f;

    glPushMatrix();
    
    
    if (lights_on){
        //White rubber
        float amb[] ={ 0.05f,0.05f,0.05f,1.0f };
        float diff[] ={ 0.5f,0.5f,0.5f,1.0f};
        float spec[] ={ 0.7f,0.7f,0.7f,1.0f};
        float shine = 10.0f;

        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,amb);
        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diff);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,spec);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,&shine);
            
    }
    else{
        glColor3f(0.7f,0.7f,0.7f);
    }
    glTexCoord2f(0,0);
    
    glEnable(GL_TEXTURE_2D);
    floorTexture.texture();
    glBegin(GL_QUADS);
        glNormal3f(0,1,0);
        
        glVertex3f(-size,0.0f,-size);//v1
        glTexCoord2f(0,1);
        glVertex3f(-size,0.0f,size);//v2
        glTexCoord2f(1,1);
        glVertex3f(size,0.0f,size);//v4
        glTexCoord2f(1,0);
        glVertex3f(size,0.0f,-size);//v3
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

// Cube with TexCoords
static void drawBox(GLfloat size, GLenum type)
{
  static GLfloat n[6][3] =
  {
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0}
  };
  static GLint faces[6][4] =
  {
    {0, 1, 2, 3},
    {3, 2, 6, 7},
    {7, 6, 5, 4},
    {4, 5, 1, 0},
    {5, 6, 2, 1},
    {7, 4, 0, 3}
  };
  GLfloat v[8][3];
  GLint i;

  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

  for (i = 5; i >= 0; i--) {
    glBegin(type);
    glNormal3fv(&n[i][0]);
    glTexCoord2f(0,0);
    glVertex3fv(&v[faces[i][0]][0]);
    glTexCoord2f(1,0);
    glVertex3fv(&v[faces[i][1]][0]);
    glTexCoord2f(1,1);
    glVertex3fv(&v[faces[i][2]][0]);
    glTexCoord2f(0,1);
    glVertex3fv(&v[faces[i][3]][0]);
    glEnd();
  }
}

glutTexturedCube(GLdouble size)
{
  drawBox(size, GL_QUADS);
}

void drawObject(Object object){
    glPushMatrix();

    glTranslatef(object.position.mX, object.position.mY, object.position.mZ);
    
    glScalef(object.scale.mX, object.scale.mY, object.scale.mZ);


    //rotation order should be zxy
    glRotatef(object.rotation.mZ*45, 0,0,1);
    glRotatef(object.rotation.mX*45, 1,0,0);
    glRotatef(object.rotation.mY*45, 0,1,0);


    if (lights_on){
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,object.material.amb);
        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,object.material.diff);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,object.material.spec);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,object.material.shiny);
    }else{
        glColor3f(object.material.diff[0], object.material.diff[1], object.material.diff[2]);
    }

    glEnable(GL_NORMALIZE);
    switch(object.type){
        case ObjectType::cube:
            glEnable(GL_TEXTURE_2D);
            brickTexture.texture();
            glutTexturedCube(2);
            glDisable(GL_TEXTURE_2D);
            break;
        case ObjectType::teapot:
            glEnable(GL_TEXTURE_2D);
            marbleTexture.texture();
            glutSolidTeapot(1);
            glDisable(GL_TEXTURE_2D);
            break;
        case ObjectType::sphere:
            glutSolidSphere(1,16,16);
            break;
        case ObjectType::cone:
            glPushMatrix();
            glRotatef(-90,1,0,0);
            glTranslatef(0.0,0.0,-1.0);
            glutSolidCone(1,2,16,16);
            glPopMatrix();
            break;
        case ObjectType::torus:
            // std::cout << "draw Donut" << std::endl;
            glutSolidTorus(0.5,1,16,16);
            break;
        case ObjectType::octahedron:
            glutSolidOctahedron();
            break;
    }
    glDisable(GL_NORMALIZE);
    // if(selectedTexture != nullptr){
    //     selectedTexture.texture();
    // }

    glPopMatrix();

}

void drawSelection(){
    glPushMatrix();
    if (lights_on){
        // Green rubber
        float amb[] ={ 0.0f,0.05f,0.0f,1.0f };
        float diff[] ={ 0.4f,0.5f,0.4f,1.0f};
        float spec[] ={0.04f,0.7f,0.04f,1.0f };
        float shine =  10.0f;

        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,amb);
        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diff);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,spec);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,&shine);
            
    }
    else{
        glColor3f(0.0,1.0,0.0);
    }

    glTranslatef(selectedObject->position.mX, selectedObject->position.mY, selectedObject->position.mZ);
    
    glScalef(selectedObject->scale.mX, selectedObject->scale.mY, selectedObject->scale.mZ);

    //rotation order should be zxy
    glRotatef(selectedObject->rotation.mZ*45, 0,0,1);
    glRotatef(selectedObject->rotation.mX*45, 1,0,0);
    glRotatef(selectedObject->rotation.mY*45, 0,1,0);

    glutWireCube(2);

    glPopMatrix();
}


void DrawScene(){
   
    

    // glPopMatrix();
    // glPushMatrix();
    

    
    //order should be zxy
    glRotatef(camera.rotation[1], 0,0,1);
    glRotatef(camera.rotation[0], 0,1,0);

    drawFloor();
    drawLight();
    drawAxis();
    
    
    for (auto object : objects){
        drawObject(object);
    }

    
    // std::cout << "draw normal" << std::endl;

    // glPushMatrix();
    // glTranslatef(cursorTo3D.mX,cursorTo3D.mY,cursorTo3D.mZ);
    // glutSolidTeapot(10);
    
    // // glutSolidTorus(0.5,1,16,16);
    // glPopMatrix();


    // glColor3f(0.3f,0.4f,0.8f);
    // glutSolidCube(10);


    // glPopMatrix();
    if (selectedObject){
        drawSelection();
    }
    


}


void printInstructions(){

    const char* instr = "No instructions yet";
    std::cout << instr << std::endl;
}

void init(void)
{
    //initialize globals 

    camera = Camera(camPos,camTarget,camUp);
    objects = std::vector<Object>();
    selectedMaterial = Material();
    // selectedObject = nullptr;
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    
	glClearColor(0.2, 0.2, 0.2, 0);
	glColor3f(1, 1, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 400);

    printInstructions();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(camera.position.mX, camera.position.mY, camera.position.mZ, camera.target.mX,camera.target.mY,camera.target.mZ, camera.upVector.mX,camera.upVector.mY,camera.upVector.mZ);
	glColor3f(0,1,0);

	
	DrawScene();

    //ray
    glPushMatrix();
    glLineWidth(5);
    glBegin(GL_LINES);
    glColor3f (1.0, 0.0, 0.0);//red is x
    glVertex3f(start[0],start[1],start[2]);
    glVertex3f(end[0],end[1],end[2]);
    glEnd();
    glPopMatrix();

	glutSwapBuffers();
}

void getModifiers(){
    alt = (glutGetModifiers() == GLUT_ACTIVE_ALT);//scale
    shift = (glutGetModifiers() == GLUT_ACTIVE_SHIFT);
    alt_shift = (glutGetModifiers() == (GLUT_ACTIVE_ALT|GLUT_ACTIVE_SHIFT));//undo scale 
}

void prepareDataForSave(float* data){


    //number of objects
    data[0] = static_cast<float>(objects.size());

    //light positions
    data[1] = lightPos[0][0];
    data[2] = lightPos[0][1];
    data[3] = lightPos[0][2];
    data[4] = lightPos[1][0];
    data[5] = lightPos[1][1];
    data[6] = lightPos[1][2];

    //objects!
    int i = 7;

    for(auto object : objects){
        std::cout<< "saving object:\nposition:" << std::endl;

        data[i] = object.position.mX; std::cout<< object.position.mX << std::endl;
        data[i+1] = object.position.mY; std::cout<< object.position.mY << std::endl;
        data[i+2] = object.position.mZ; std::cout<< object.position.mZ << std::endl;
        data[i+3] = object.rotation.mX; std::cout<< "rotation:\n"<<object.rotation.mX << std::endl;
        data[i+4] = object.rotation.mY;std::cout<< object.rotation.mY << std::endl;
        data[i+5] = object.rotation.mZ;std::cout<< object.rotation.mZ << std::endl;
        data[i+6] = object.scale.mX; std::cout<< "scale:\n"<<object.scale.mX << std::endl;
        data[i+7] = object.scale.mY;std::cout<< object.scale.mY << std::endl;
        data[i+8] = object.scale.mZ;std::cout<< object.scale.mZ << std::endl;
        data[i+9] = static_cast<float>(object.material.type);
        data[i+10] = static_cast<float>(object.type);
        i+=11;
    }

}

void kbd(unsigned char key, int x, int y)
{
    std::string input;
    
    //for saving
    int dataSize = 7+11*objects.size();
    float data[dataSize];

    //for loading
    int loadedDataSize;
    float loadedData[loadedDataSize];


    switch(key){
        case 'S' :
        case 's' :
            std::cout << "enter the filename including extension .txt"  << std::endl;
            std::cin >> input;

            
            prepareDataForSave(data);
            

            FileHandler::saveFile(input,data,dataSize);

        break; 

        case 'O':
        case 'o':
            std::cout << "enter the filename to open including extension .txt"  << std::endl;
            std::cin >> input;

            FileHandler::loadFile(input,loadedData,loadedDataSize);

            loadScene(loadedData);
        break;

        case 'R':
            resetScene();
            break;

        case 27:
            exit(0);
            break;
        case '[':
            selectedLight = 1;
            break;
        case ']':
            selectedLight = 0;
            break;
        case 'l':
        case 'L':
            lights_on = !lights_on;
            if(lights_on){
                glEnable(GL_LIGHTING);
                glEnable(GL_LIGHT0);
                glEnable(GL_LIGHT1);
            }else{
                glDisable(GL_LIGHT0);
                glDisable(GL_LIGHT1);
                glDisable(GL_LIGHTING);
            }
        break;
        case '1':
            
            selectedMaterial = Material(MaterialType::plastic);
            if (selectedObject){
                selectedObject->material = selectedMaterial;
            }
            
            break;
        case '2':
            selectedMaterial = Material(MaterialType::emerald);
            if (selectedObject){
                selectedObject->material = selectedMaterial;
            }
            break;
        case '3':
            selectedMaterial = Material(MaterialType::ruby);
            if (selectedObject){
                selectedObject->material = selectedMaterial;
            }
            break;
        case '4':
            selectedMaterial = Material(MaterialType::gold);
            if (selectedObject){
                selectedObject->material = selectedMaterial;
            }
            break;
        case '5':
            selectedMaterial = Material(MaterialType::pearl);
            if (selectedObject){
                selectedObject->material = selectedMaterial;
            }
            break;
        case '6':
            selectedTexture = marbleTexture;
            break;
        case 'q':
            objects.push_back(Object(selectedMaterial, ObjectType::cube));
            selectedObject = &objects.back();
            break;
        case 'w':
            objects.push_back(Object(selectedMaterial, ObjectType::teapot));
            selectedObject = &objects.back();
            break;
        case 'e':
            objects.push_back(Object(selectedMaterial, ObjectType::sphere));
            selectedObject = &objects.back();
            break;
        case 'r':
            objects.push_back(Object(selectedMaterial, ObjectType::cone));
            selectedObject = &objects.back();
            break;
        case 't':
            objects.push_back(Object(selectedMaterial, ObjectType::torus));
            selectedObject = &objects.back();
            break;
        case 'y':
            objects.push_back(Object(selectedMaterial, ObjectType::octahedron));
            selectedObject = &objects.back();
            break;


        case 'Z':
        case 'z': // z axis translation

            if (!selectedObject){break;}

            getModifiers();


            if (shift){
                selectedObject->position.mZ--;
            }else if(alt){
                selectedObject->scale.mZ++;
            }else if (alt_shift){
                selectedObject->scale.mZ--;
            }else{
                selectedObject->position.mZ++;
            }
            break;
        case 'X':
        case 'x': // x axis translation

            if (!selectedObject){break;}

            getModifiers();

            if (shift){
                selectedObject->position.mX--;
            }else if(alt){
                selectedObject->scale.mX++;
            }else if (alt_shift){
                selectedObject->scale.mX--;
            }else{
                selectedObject->position.mX++;
            }
            break;
        case 'C':
        case 'c': // y axis translation

            if (!selectedObject){break;}

            getModifiers();

            if (shift){
                selectedObject->position.mY--;
            }else if(alt){
                selectedObject->scale.mY++;
            }else if (alt_shift){
                selectedObject->scale.mY--;
            }else{
                selectedObject->position.mY++;
            }
            break;



        case 'B':
            selectedObject->rotation.mZ = (static_cast<int>(selectedObject->rotation.mZ) - 1)%8;
        break;
        case 'b':
            selectedObject->rotation.mZ = (static_cast<int>(selectedObject->rotation.mZ) + 1)%8;
        break;
        case 'N':
            selectedObject->rotation.mX = (static_cast<int>(selectedObject->rotation.mX) - 1)%8;
        break;
        case 'n':
            selectedObject->rotation.mX = (static_cast<int>(selectedObject->rotation.mX) + 1)%8;
        break;
        case 'M':
            selectedObject->rotation.mY = (static_cast<int>(selectedObject->rotation.mY) - 1)%8;
        break;
        case 'm':
            selectedObject->rotation.mY = (static_cast<int>(selectedObject->rotation.mY) + 1)%8;
        break;

    }
}

void specialkbd(int key, int x, int y){
   switch(key){
        case GLUT_KEY_DOWN:
            if (selectedLight == 0){
                lightPos[0][0]++;
            }else{
                lightPos[1][0]++;
            }
            
            //globalRot[2]--;
        break;
        case GLUT_KEY_UP:
            //lightPos[0]++; 
            if (selectedLight == 0){
                lightPos[0][0]--;
            }else{
                lightPos[1][0]--;
            }
            //globalRot[2]++;
        break;
        case GLUT_KEY_LEFT:
            //lightPos[2]--; 
            if (selectedLight == 0){
                lightPos[0][2]++;
            }else{
                lightPos[1][2]++;
            }
            //globalRot[1]--;
        break;
        case GLUT_KEY_RIGHT: 
            //lightPos[2]++;
            if (selectedLight == 0){
                lightPos[0][2]--;
            }else{
                lightPos[1][2]--;
            }
            //globalRot[1]++;
        break;
    }
}

Vec3D computeMouseRay(int x, int y){
    //adapted from https://nehe.gamedev.net/article/using_gluunproject/16013/

    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ; // click coords

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    winX = (float)x;
    winY = (float)y;

    // glReadPixels(x,int(winY),1,1,GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    // std::cout<<winZ<<std::endl;

    //near plane
    gluUnProject(winX,winY,0.0,modelview,projection,viewport,&start[0],&start[1],&start[2]);

    //far plane
    gluUnProject(winX,winY,1.0,modelview,projection,viewport,&end[0],&end[1],&end[2]);

    std::cout << "Ray start" << std::endl;
    std::cout<<start[0] << " " << start[1] << " " << start[2]<<std::endl;
    std::cout << "Ray End"<< std::endl;
    std::cout<<end[0] << " " << end[1] << " " << end[2]<<std::endl;
    // glPushMatrix();
    // glLoadIdentity();
    // glLineWidth(5);
    // glBegin(GL_LINES);
    // glColor3f (1.0, 0.0, 0.0);//red is x
    // glVertex3f(start[0],start[1],start[2]);
    // glVertex3f(end[0],end[1],end[2]);
    // glEnd();
    // glPopMatrix();
    Vec3D newRay = Vec3D::createVector(Point3D(start[0],start[1],start[2]),Point3D(end[0],end[1],end[2]));
    newRay.start = Point3D(start[0], start[1], start[2]); 

    return newRay;
}


void mouse(int button, int state, int x , int y){
    y = window_size_y - y; // flip y

  //  std::cout << "Mouse Coordinates: ";
   // std::cout << x << ", " << y << std::endl; // endline is neccassary for terminal to print line by line


    if (button == GLUT_LEFT_BUTTON) // LMB pressed
    {
      //  std::cout << "LEFT BUTTON" << std::endl;
        if (state == GLUT_UP)
        {
     //       std::cout << "RELEASED!" << std::endl;
            leftClick = false;
        }
        if (state == GLUT_DOWN)
        {
            
      //      std::cout << "PRESSED!" << std::endl;
            leftClick = true;

            cursorRay = computeMouseRay(x,y);
            Point3D intercept = rayPlaneIntersection(cursorRay, ground);
            std::cout << "Intercept" << std::endl;
            std::cout<< intercept.mX << " " << intercept.mY << " " << intercept.mZ <<std::endl;
            


        }
    }

    else if (button == GLUT_RIGHT_BUTTON) // RMB pressed
    {
      //  std::cout << "RIGHT BUTTON" << std::endl;
        if (state == GLUT_UP)
        {
       //     std::cout << "RELEASED!" << std::endl;
            rightClick = false;
            previousRightClickPosition[0] = camera.rotation[0]; // y position of mouse
            previousRightClickPosition[1] = camera.rotation[1]; // x position of mouse
        }
        if (state == GLUT_DOWN)
        {
          //  std::cout << "PRESSED!" << std::endl;
            rightClick = true;
            initialRightClickPosition = Point3D(x,y,0);

        }
    }

    else if(button == 3){//mouse scroll up
        if (state == GLUT_UP){return;}

        // std::cout << "SCROLL UP!" << std::endl;

        camera.zoomIn();

    }

    else if(button == 4){//mouse scroll down
        if (state == GLUT_UP){return;}

    //    std::cout << "SCROLL DOWN!" << std::endl;

        camera.zoomOut();

    }
}

void motion(int x, int y){
    y = window_size_y - y; // flip y

    if(rightClick){
        Vec3D clickDistance = Vec3D::createVector(initialRightClickPosition, Point3D(x,y,0));
        camera.rotation[0] = clickDistance.mX + previousRightClickPosition[0]; // y axis rotation depends on x coords of mouse
        camera.rotation[1] = clickDistance.mY + previousRightClickPosition[1]; // x axis rotation depends on y coords of mouse
    }

}

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


int main(int argc, char **argv){
    marbleTexture.load("texture/marble.ppm");
    floorTexture.load("texture/floor.ppm");
    brickTexture.load("texture/snake.ppm");

    // glGenTextures(0, &marbleTexture);
    // glGenTextures(1, &floorTexture);
    // glGenTextures(2, &brickTexture);

    glutInit(&argc, argv);		//starts up GLUT
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    
	
	glutInitWindowSize(window_size_x, window_size_y);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("3D model");	//creates the window

	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(kbd);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutSpecialFunc(specialkbd);
    glutTimerFunc(17,update,0);
    glutReshapeFunc(windowReshapeFunc);

	glEnable(GL_DEPTH_TEST);
    
    // glEnable(GL_TEXTURE_2D);
    
	glFrontFace(GL_CW);
	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);
    
	init();

	glutMainLoop();				//starts the event loop


    return 0;
}