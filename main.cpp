
// EECE 478
// Written by Jackie Chen
// Student Number: 60006087

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <math.h>
#include <string>
#include <iostream>
#include "model.h"
#include "PPM.h"
#include "City.h"

#define WIDTH 1000 // Width of window
#define HEIGHT 700 // Height of window
#define NEAR 1.0f // Nearest for reshape function
#define FAR 5000.0f // Furthest for reshape function
#define VIEWANGLE 60.0f // Viewing angle for reshape function
#define WINDOW_X 200 // Top left X pos of window
#define WINDOW_Y 100 // Top left Y pos of window

#define CAMERASPEED 5.0f // DO NOT MODIFY - DOES NOT WORK PROPERLY
#define SHUTTLE 2 // Index of shuttle model

using namespace std;

void lighting(void);
void display(void);
void reshape(int, int);
void idle(void);
void normalkeyinput(unsigned char, int, int);
void computepos_fb(float);
void computepos_ud(float);
void computepos_lr(float);
void computedir_lr(float);
void computedir_ud(float);
void pressnormalkey(unsigned char, int, int);
void releasenormalkey(unsigned char, int, int);
void mousebutton(int,int,int,int);
void mousemove(int,int);
void drawtext(float, float, const string &);
void computecamera(void);
void computerotation(void);
void captureScene(int,int);
void predefined_camera(void);
void fog(void);

City wonderland;

float light_x = 0.0f;
float light_z = 0.0f;
float light_angle = 0;
int light_flash = 0;
float light_pos = 0;
bool lightning = false;
bool light_on = true;

float angle = 0.0f;
float lx = 0.0f;
float lz = 0.0f;
float ly = 0.0f;
float x = 0.0f;
float z = 0.0f;
float y = 500.0f;
float sx = 0.0f;
float sz = 0.0f;
float deltaMove_fb = 0.0f;
float deltaMove_lr = 0.0f;
float deltaMove_ud = 0.0f;
float deltaAngle_lr = 0.0f;
float deltaAngle_ud = 0.0f;
int xOrigin = -1;
float mouse_x_angle = 0.0f;
int yOrigin = -1;
float mouse_y_angle = 0.0f;
bool wire_frame = false;
string building_name;

char* cwd;

bool moveShuttle = false;
bool followShuttle = false;
bool followShuttleTop = false;
float ShuttleSpeed = 15.0f;

bool capturing = false;
bool force_noCap = false;
int frame_num = 0;

bool predefCam = false;
int camStage = 0;
int camCounter = 0;
bool introduction = false;
bool ending = false;

// Function to enable and set fog parameters
void fog(void) {
    glPushMatrix();
    GLfloat density = 0.3;
    GLfloat fogColor[4] = {0.0f,0.0f,0.0f, 1.0f}; 
    glEnable (GL_FOG);
    glFogi (GL_FOG_MODE, GL_LINEAR);
    glFogfv (GL_FOG_COLOR, fogColor);
    glFogf (GL_FOG_DENSITY, density);
    glHint (GL_FOG_HINT, GL_NICEST);
    glFogf(GL_FOG_START, 1500.0);
    glFogf(GL_FOG_END, 2000.0f);
    glPopMatrix();
}

// Function to enable and compute lighting
void lighting(void) {
    
    glPushMatrix();

    glEnable(GL_LIGHTING);
    
    // Ambient Lighting
    GLfloat global_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    
    // Lighting from camera
    if (light_on == true) glEnable(GL_LIGHT0);
    else glDisable(GL_LIGHT0);
    GLfloat light0_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light0_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light0_position[] = { x, y, z, 1.0f};
    GLfloat light0_direction[] = { lx, ly, lz};
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0001);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.00001);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.00001);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_direction);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 128.0f);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF,8.0f); 
    
    
    // Lighting from sky
    glEnable(GL_LIGHT1);
    GLfloat light1_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    GLfloat light1_position[] = { 650.0f, 1000.0f, light_pos, 1.0f};
    GLfloat light1_direction[] = { 0.0f,-1.0f,0.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.001f);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0001f);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0f);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60.0);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_direction);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 1.0); 
    
    
    // Lightning flash
    if ((light_flash >= 200 && light_flash <= 205) || (light_flash >= 208 && light_flash <= 213)
        || (light_flash >= 219 && light_flash <= 226)) {
        lightning = true;
        glEnable(GL_LIGHT2);
        GLfloat light2_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
        GLfloat light2_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat light2_specular[] = { 1.0, 1.0, 1.0, 1.0 };
        glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
        glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
        GLfloat light2_position[] = { 0.0f,1000.0f,0.0f,1.0f};
        GLfloat light2_direction[] = { 0.0f,-1.0f,0.0f };
        glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
        glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.0);
        glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.0);
        glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.0);
        glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 120.0);
        glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_direction);
        glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 128.0); 
    } else {
        lightning = false;
        glDisable(GL_LIGHT2);
    }

    // Lighting from shuttle
    if (moveShuttle == true) glEnable(GL_LIGHT3);
    else glDisable(GL_LIGHT3);
    GLfloat light3_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light3_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light3_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light3_position[] = { wonderland.models[2]->translate_x, wonderland.models[2]->translate_y + 100.0f, wonderland.models[2]->translate_z, 1.0f};
    GLfloat light3_direction[] = { 0.0f, -1.0f, 0.0f };
    glLightfv(GL_LIGHT3, GL_AMBIENT, light3_ambient);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, light3_diffuse);
    glLightfv(GL_LIGHT3, GL_SPECULAR, light3_specular);
    glLightfv(GL_LIGHT3, GL_POSITION, light3_position);
    glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 0.0001);
    glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.00001);
    glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.000001);
    glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light3_direction);
    glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 128.0f);
    glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 45.0f); 
    
    
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_AMBIENT,GL_DIFFUSE);
    glShadeModel(GL_SMOOTH);
/*
    glColor3f(0.0f,0.0f,1.0f);
    glTranslatef(500*cos(light_angle), 3500.0f, 500*sin(light_angle));
    glutSolidSphere(100.0f,50,50);

     if (light_angle >= 2*3.14) light_angle = 0;
     else light_angle += (2*3.14)/200.0f;  
   */
    
    if (light_pos > 8000) {
        light_pos = -3000;
    } else {
        light_pos += 10.0f;
    }
    if (light_flash == 450) {
        light_flash = 0;
    } else
        light_flash++;
    glPopMatrix();

}

// Function to draw text
void drawtext(float x, float y, const string &s) {
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

    // Draw the textat the specified position
	const char *c = s.c_str();
	glRasterPos2f(x, y);
	for(int i = 0; i < s.size(); i++) {
        if (ending || introduction)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,c[i]);
        else
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15,c[i]);
    }

    // Restore
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
}

// Display Function
void display(void) {
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f,0.00f,0.0f,1.0f);
    if (lightning) {
        glEnable(GL_BLEND);
        glBlendFunc (GL_ONE,GL_ONE);
    }
    
    // If ending, render nothing but the ending credits
    if (ending) {
        // Set the matrix mode to modelview to render the model
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        glColor4f(0.8f,0.0f,0.0f,1.0f);
        drawtext(-0.1f, 0.50f, "THE END");
        drawtext(-0.2f, 0.20f, "DIRECTED BY:");
        drawtext(0.0f, -0.10f, "JACKIE CHEN");
    } else {
        // Draw name and student number on the display, or introduction text
        if (introduction) {
            glColor4f(0.8f,0.0f,0.0f,1.0f);
            drawtext(-0.2f, 0.40f, "MOVIE BY:");
            drawtext(0.0f, 0.10f, "JACKIE CHEN");
        } else {
            glColor4f(0.0f,0.8f,0.5f,1.0f);
            drawtext(0.70f, -0.95f, "Jackie Chen");
        }
        
        // Set the matrix mode to modelview to render the model
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        // Compute the object's rotation based on the mouse events
        computerotation();
        
        // Draw in either wire frame or polygon (fill)
        if (wire_frame == true) glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        else glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        
        // Compute the camera's parameters
        computecamera();
        
        //cout << "(x,y,z): " << "(" << x << "," << y << "," << z << ")" << endl;
        //cout << "(lx,ly,lz): " << "(" << lx << "," << ly << "," << lz << ")" << endl;
        
        lighting();
        fog();
                
        // Render city and animate shuttle, and also pass camera coordinates to the city for infinite terrain purposes
        wonderland.xPos = x;
        wonderland.yPos = y;
        wonderland.zPos = z;
        wonderland.renderCity();
        if (moveShuttle)
            wonderland.animateShuttle();
    }
    
    if (lightning) {
        glColor4f(1.0f,1.0f,1.0f,0.5f);
        glBegin(GL_QUADS);
        glVertex2f(0.0f,0.0f);
        glVertex2f(0.0f,700.0f);
        glVertex2f(1000.0f,0.0f);
        glVertex2f(1000.0f,700.0f);
        glEnd();
    } else {
        glDisable(GL_BLEND);
    }
    
    glFlush();
    // Swap buffers to display the model on the screen
    glutSwapBuffers();
}

// Function to capture the current frame to a PPM
void captureScene(int width, int height)
{
    GLint w = width;
    GLint h = height;
    unsigned char* img = new unsigned char[3*(w+1)*(h+1)+3];
    char buffer[4];
    sprintf(buffer, "%d", frame_num);
    string framenum = "";

    for (int i = 0; i < 4; i++) {
        if (buffer[i] >= '0' && buffer[i] <= '9')
            framenum += buffer[i];
    }
    
    string curdir = cwd;
    string path;
    if (introduction)
         path = curdir + "/Models/" + wonderland.getCityName() + "/Capture/intro_" + framenum + ".ppm";
    else if (ending) 
        path = curdir + "/Models/" + wonderland.getCityName() + "/Capture/end_" + framenum + ".ppm";
    else
        path = curdir + "/Models/" + wonderland.getCityName() + "/Capture/" + framenum + ".ppm";
    cout << path << endl;
    glReadPixels((GLint)0, (GLint)0,(GLint)w-1,(GLint)h-1,GL_RGB, GL_UNSIGNED_BYTE,img);
    PPM::Write(path,img,w,h);
    delete[] img;
    frame_num++;
}

// Reshape function to ensure object does not distort in shape.
void reshape(int w, int h) {
    // If height is 0, set to default of 1 (to avoid division by 0)
	if (h == 0) h = 1;
	
    float ratio =  1.0 * (w/h);
    
    //Specify the window size parameters and the field of view
    glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(VIEWANGLE, ratio, NEAR, FAR);
	glMatrixMode(GL_MODELVIEW);
}

// Key inputs
void pressnormalkey(unsigned char key, int x, int y) {
	switch (key) {  
		case 27:    
            exit(0);    break; //ESC
        case 97:    
            deltaAngle_lr = -0.0646f;               break; //a - TURN LEFT 
        case 100:   
            deltaAngle_lr = 0.0646f;                break; //d - TURN RIGHT
        case 115:   
            deltaMove_fb = -180.3117f;              break; //s - GO BACKWARD 
        case 119:   
            deltaMove_fb = 180.3117f;               break; //w - GO FORWARD
        case 102:   
            deltaMove_ud = -8.02f;                  break; //f - GO DOWN
        case 114:  
            deltaMove_ud = 8.02f;                   break; //r - GO UP
        case 101:  
            deltaMove_lr = 12.03f;                  break; //e - STRAFE RIGHT
        case 113:   
            deltaMove_lr = -12.03f;                 break; //q - STRAFE LEFT
        case 116:  
            deltaAngle_ud = 0.112f;                 break; //t - LOOK UP
        case 103:  
            deltaAngle_ud = -0.112f;                break; //g - LOOK DOWN
        case 39:   
            ly = 0.0f; lx = 0.0f; lz = -1.0f; 
            x = 0.0f; y = 1.0f; z = 5.0f;           break; //' - RESET CAMERA POSITION
        case 112:   
            if (wire_frame == true) wire_frame = false;
            else wire_frame = true;     
            if (wire_frame == true) glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            else glPolygonMode(GL_FRONT,GL_FILL);   break; //p - WIREFRAME OR POLYGON
        case 108:   
            if (light_on == true) light_on = false;
            else {
                light_on = true;
                cout << "lights on!\n";
            }                                       break; //l - Camera light
        case 106: 
            if (moveShuttle) moveShuttle = false;
            else moveShuttle = true;                break; //j - Animate shuttle
        case 107: 
            if (followShuttle) followShuttle = false;
            else followShuttle = true;              break; //k - Follow shuttle
        case 61:
            ShuttleSpeed += 2.0f;
            wonderland.setShuttleSpeed(ShuttleSpeed);
                                                    break; //= - Increase shuttle speed
        case 45:
            ShuttleSpeed -= 2.0f;
            wonderland.setShuttleSpeed(ShuttleSpeed);
                                                    break; //- - Decrease shuttle speed
        case 105: 
            if (force_noCap) force_noCap = false;
            else force_noCap = true;                break; //i - Start capturing to disk
        case 117:
            if (predefCam) {
                introduction = false;
                ending = false;
               predefCam = false;
            }
            else {
                introduction = true;
                predefCam = true;  
            }                                       break; //u - Start predefined camera path

	}
}

// If key press has been released, stop computations.
void releasenormalkey(unsigned char key, int x, int y) {
	switch (key) {
        case 97:    deltaAngle_lr = 0.0f;         break; 
        case 100:   deltaAngle_lr = 0.0f;         break; 
        case 115:   deltaMove_fb = 0.0f;          break; 
        case 119:   deltaMove_fb = 0.0f;          break; 
        case 102:   deltaMove_ud = 0.0f;          break; 
        case 114:   deltaMove_ud = 0.0f;          break; 
        case 101:   deltaMove_lr = 0.0f;          break; 
        case 113:   deltaMove_lr = 0.0f;          break; 
        case 116:   deltaAngle_ud = 0.0f;         break; 
        case 103:   deltaAngle_ud = 0.0f;         break; 
	}
}

// Mouse Click. See if mouse has been pressed down or has been released.
void mousebutton(int button, int state, int x, int y) {
    // If left mouse button has been clicked, set x and y origin for computation of mouse movement
	if (button == GLUT_LEFT_BUTTON) {
        xOrigin = x;
        yOrigin = y;
	}
}


// Mouse Movement. Rotate the object depending on the mouse movement.
void mousemove(int x, int y) {
	if (xOrigin >= 0) 
        mouse_y_angle += (xOrigin - x)/45; // Set angle around y axis if mouse has been moved left or right
    if (yOrigin >= 0)
        mouse_x_angle += (yOrigin - y)/45; // Set angle around x axis if mouse has been moved up or down
}

// Compute and Set Camera Parameters
void computecamera(void) {
    // Compute the parameters
    if (deltaMove_fb) computepos_fb(deltaMove_fb);
    if (deltaMove_ud) computepos_ud(deltaMove_ud);
    if (deltaMove_lr) computepos_lr(deltaMove_lr);
	if (deltaAngle_lr) computedir_lr(deltaAngle_lr);
    if (deltaAngle_ud) computedir_ud(deltaAngle_ud);
    
    // Set camera parameters
    // Normal camera mode
    if (!followShuttle && !followShuttleTop)
        gluLookAt(	x, y, z, 
              x+lx, y+ly,  z+lz, 
              0.0f, 1.0f,  0.0f);
    
    // Function to follow the shuttle from a top down view
    else if (followShuttleTop) {
        gluLookAt(  wonderland.models[2]->translate_x, wonderland.models[2]->translate_y + 450, wonderland.models[2]->translate_z, 
                  0.0f, -100000.0f, 0.0f, 
                  0.0f, 1.0f,  0.0f);
    }
    // Function to follow shuttle from third person view (behind)
    else {
        if (wonderland.models[2]->rot_y == -90)
            gluLookAt(	wonderland.models[2]->translate_x + 30, wonderland.models[2]->translate_y + 100, wonderland.models[2]->translate_z + 90, 
                      wonderland.models[2]->translate_x + 30, wonderland.models[2]->translate_y,  wonderland.models[2]->translate_z - 1, 
                      0.0f, 1.0f,  0.0f);
        else if (wonderland.models[2]->rot_y == 0)
            gluLookAt(	wonderland.models[2]->translate_x + 90, wonderland.models[2]->translate_y + 100, wonderland.models[2]->translate_z - 30, 
                      wonderland.models[2]->translate_x - 1, wonderland.models[2]->translate_y,  wonderland.models[2]->translate_z - 30, 
                      0.0f, 1.0f,  0.0f);
        else if (wonderland.models[2]->rot_y == 180)
            gluLookAt(	wonderland.models[2]->translate_x - 90, wonderland.models[2]->translate_y + 100, wonderland.models[2]->translate_z + 30, 
                      wonderland.models[2]->translate_x + 1, wonderland.models[2]->translate_y,  wonderland.models[2]->translate_z + 30, 
                      0.0f, 1.0f,  0.0f);
        else if (wonderland.models[2]->rot_y == 90)
            gluLookAt(	wonderland.models[2]->translate_x - 30, wonderland.models[2]->translate_y + 100, wonderland.models[2]->translate_z - 90, 
                      wonderland.models[2]->translate_x - 30, wonderland.models[2]->translate_y,  wonderland.models[2]->translate_z + 1, 
                      0.0f, 1.0f,  0.0f);
    }
}

// Compute the object's rotation/angle
void computerotation(void) {
    // Rotate object based on mouse movement
    glRotatef(mouse_y_angle,0,1,0);
    glRotatef(mouse_x_angle,1,0,0);
}

// Compute position of camera (move front or backwards)
void computepos_fb(float deltaMove_fb) {
	x += deltaMove_fb * lx * 0.1f;
	z += deltaMove_fb * lz * 0.1f;
    y += deltaMove_fb * ly * 0.1f;
}

// Compute position of camera (move up or down on y axis)
void computepos_ud(float deltaMove_ud) {
    y += deltaMove_ud;
}

// Compute position of camera (move left or right)
void computepos_lr(float deltaMove_lr) {
    x += -lz * deltaMove_lr;
    z += lx * deltaMove_lr;
}

// Compute centerpoint focus of camera (left or right)
void computedir_lr(float deltaAngle_fb) {
	angle += deltaAngle_fb;
	lx = sin(angle);
	lz = -cos(angle);
}

// Compute centerpoint focus of camera (up or down)
void computedir_ud(float deltaAngle_ud) {
    ly += deltaAngle_ud;
}

void idle(void) {
    // Predefined camera path
    if (predefCam)
        predefined_camera();
    
    // Capture frames to disk
    if (capturing && !force_noCap)
        captureScene(WIDTH,HEIGHT);

    glutPostRedisplay();
}

void predefined_camera(void) {
    
    if (introduction) {
        switch(camStage) {
            case 0:
                wonderland.models[SHUTTLE]->translate_x = 0;
                wonderland.models[SHUTTLE]->translate_y = 600;
                wonderland.models[SHUTTLE]->translate_z = -10000;
                followShuttle = true;
                capturing = true;
                camStage = 1;
                break;
            case 1:
                wonderland.introductoryShuttle(20.0f);
                x = wonderland.models[SHUTTLE]->translate_x;
                z = wonderland.models[SHUTTLE]->translate_z;
                if (wonderland.models[SHUTTLE]->translate_z < - 10300) {
                    camStage = 2;
                }
                break;
            case 2:
                wonderland.introductoryShuttle(60.0f);
                x = wonderland.models[SHUTTLE]->translate_x;
                z = wonderland.models[SHUTTLE]->translate_z;
                if (wonderland.models[SHUTTLE]->translate_z < - 12000) {
                    camStage = 3;
                }
                break;
            case 3:
                wonderland.introductoryShuttle(120.0f);
                x = wonderland.models[SHUTTLE]->translate_x;
                z = wonderland.models[SHUTTLE]->translate_z;
                if (wonderland.models[SHUTTLE]->translate_z < - 18000) {
                    camStage = 4;
                }
                break;
            default:
                camStage = 0;
                wonderland.models[SHUTTLE]->translate_x = 670;
                wonderland.models[SHUTTLE]->translate_y = 20;
                wonderland.models[SHUTTLE]->translate_z = 865;
                frame_num = 0;
                introduction = false;
                followShuttle = false;
                capturing = false;
                break;
        }
    } else {
        switch(camStage) {
            case 0 :
                x = -358; y = 852; z = 320;
                ly = -0.784;
                angle = 1.97f;
                moveShuttle = true;
                camCounter++;
                if (camCounter == 400) {
                    capturing = true;
                    camStage = 1;
                    camCounter = 0;
                }
                break;
            case 1:
                x += .3096f*CAMERASPEED; z += 4.0f*CAMERASPEED;
                angle += -0.01f;
                if (z > 1000) {
                    camStage = 2;
                }
                break;
            case 2:
                x += .3096f*CAMERASPEED; y -= 0.5f*CAMERASPEED; z += 4.0f*CAMERASPEED;
                ly += 0.0002f*CAMERASPEED;
                angle += -0.018f;
                if (z > 1300) {
                    camStage = 3;
                }
                break;
            case 3:
                x += .2296f*CAMERASPEED; y -= 1.5f*CAMERASPEED; z += 1.5f*CAMERASPEED;
                ly += 0.0005f*CAMERASPEED;
                angle += -0.005f;
                if (y < 350) {
                    camStage = 4;
                }
                break;
            case 4:
                x += 2.1296f*CAMERASPEED; y += 0.01f*CAMERASPEED; z -= 0.5f*CAMERASPEED;
                ly += 0.0f*CAMERASPEED;
                angle += -0.01f;
                if (x > 100) {
                    camStage = 5;
                }
                break;
            case 5:
                x += 2.1296f*CAMERASPEED; y -= 0.3f*CAMERASPEED; z += 0.3f*CAMERASPEED;
                ly += 0.0004f*CAMERASPEED; 
                angle += -0.013f;
                if (x > 1200) {
                    camStage = 6;
                }
                break;
            case 6:
                x += 1.3296f*CAMERASPEED; y += 0.1f*CAMERASPEED; z -= 1.7f*CAMERASPEED;
                ly += 0.0011f*CAMERASPEED;
                angle += -0.016f;
                if (z < 1250) {
                    camStage = 7;
                }
                break;
            case 7:
                x += 1.1296f*CAMERASPEED; y += 0.1f*CAMERASPEED; z -= 1.7f*CAMERASPEED;
                ly -= 0.0003f*CAMERASPEED;
                angle += 0.006f;
                if (z < 850) {
                    camStage = 8;
                }
                break;
            case 8:
                x -= 1.0296f*CAMERASPEED*2; y += 0.01f*CAMERASPEED; z -= 0.8f*CAMERASPEED*2;
                ly -= 0.001f*CAMERASPEED;
                angle += 0.002f;
                if (z < 600) {
                    camStage = 9;
                }
                break;
            case 9:
                x -= 2.6296f*CAMERASPEED; y -= 0.001f*CAMERASPEED; z += 0.4f*CAMERASPEED;
                ly -= 0.0f*CAMERASPEED;
                angle += 0.018f;
                if (x < 980) {
                    wonderland.ShuttlePoint = 100;
                    moveShuttle = false;
                    camStage = 10;
                }
                break;
            case 10:
                x -= 0.0296f*CAMERASPEED; y -= 0.01f*CAMERASPEED; z -= 0.8f*CAMERASPEED;
                ly -= 0.0f*CAMERASPEED;
                angle += 0.03f;
                if (z < 170) {
                    moveShuttle = true;
                    camStage = 11;
                }
                break;
            case 11:
                x -= 0.0496f*CAMERASPEED; y -= 0.015f*CAMERASPEED; z += 2.5f*CAMERASPEED;
                ly -= 0.0f*CAMERASPEED;
                angle += 0.01f;
                if (z > 355) {
                    camStage = 12;
                }
                break;
            case 12:
                camCounter++;
                if (camCounter > 6) {
                    followShuttle = true;
                    camCounter = 0;
                    camStage = 13;
                }
                break;
            case 13:
                camCounter++;
                if (camCounter > 200) {
                    followShuttle = false;
                    followShuttleTop = true;
                    camCounter = 0;
                    camStage = 14;
                }
                break;
            case 14:
                camCounter++;
                if (camCounter > 495) {
                    moveShuttle = false;
                    camCounter = 0;
                    camStage = 15;
                }
                break;
            case 15:
                camCounter++;
                if (camCounter > 15) {
                    frame_num = 0;
                    ending = true;
                    camCounter = 0;
                    camStage = 16;
                }
                break;
            case 16:
                camCounter++;
                if (camCounter > 20) {
                    capturing = false;
                }
                break;
            default:
                break;
        }
    }
	lx = sin(angle);
	lz = -cos(angle);    
    
}

// Main Function. Initiates all GLUT Components.
int main(int argc, char **argv) {
    
    // Get the CWD and store it
    cwd = getcwd(NULL,0);

    // Initialize the GLUT parameters and functions
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(WINDOW_X,WINDOW_Y);
	glutInitWindowSize(WIDTH,HEIGHT);
	glutCreateWindow("Jackie's Renderer");
    
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    string cityname;
    cout << "Enter city to load: ";
    cin >> cityname;
    
    wonderland.loadcity(cityname);
    wonderland.setShuttleSpeed(ShuttleSpeed);

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(pressnormalkey);
    glutIgnoreKeyRepeat(1);
    glutKeyboardUpFunc(releasenormalkey);
    glutMouseFunc(mousebutton);
    glutMotionFunc(mousemove);
    glutMainLoop();
    return EXIT_SUCCESS;
}