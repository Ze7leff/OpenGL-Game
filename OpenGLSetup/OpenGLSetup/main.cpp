/*******************************************************************
           Hierarchical Multi-Part Model Example
********************************************************************/
//FOR MAC COMPILATION
#include <GL/glew.h>

#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <string.h>
#include <math.h>
//#include <glut/glut.h>
#include <GL/freeglut.h>
#include "VECTOR3D.h"
#include "cube.h"
//#include <CoreGraphics/CoreGraphics.h>
//#include <OpenGL/gl.h>
#include <iostream>
//#include <C:\Users/User\Downloads\RoboGame-main\RoboGame-main\ImageIO\ImageIO.h>
#include "C:\Users\User\Downloads\RoboGame-main\RoboGame-main\texturecube.c" // Replace with the correct header file name
#include <errno.h>
#include <sys/stat.h>
#include <math.h>
//#include "/Users/User/Downloads/22_polyMesh-TextureMapping-Example/RGBpixmap.h"


#define GL_SILENCE_DEPRECATION
using namespace std;
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
/*
 //FOR WINDOWS COMPILATION
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <gl/glut.h>
#include "VECTOR3D.h"
#include "cube.h"
#include <vector>
#include <GL/freeglut.h>
//#include <GL/glew.h>
using namespace std;
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
*/
// Note how everything depends on robot body dimensions so that can scale entire robot proportionately
// just by changing robot body scale
const int vWidth = 650;    // Viewport width in pixels
const int vHeight = 500;    // Viewport height in pixels
RGBpixmap pix1[1];
GLfloat yang = 0.0;
GLfloat xang = 0.0;
double const pi = 3.1415926;

float cannonAngleX = 0.0f; // Rotation around X-axis
float cannonAngleY = 0.0f; // Rotation around Y-axis
float mouseSensitivity = 0.1f; // Adjust sensitivity to control speed of movement

//manipulate camera
float LookX = 0.0;
float LookY = 10.0;
float LookZ = 22.0;
float robotBodyWidth = 8.0;
float robotBodyLength = 10.0;
float robotBodyDepth = 6.0;
float headWidth = 0.4 * robotBodyWidth;
float headLength = headWidth;
float headDepth = headWidth;
float upperArmLength = 0.5 * robotBodyLength;
float upperArmWidth = 0.125 * robotBodyWidth;
float gunLength = upperArmLength / 4.0;
float gunWidth = upperArmWidth;
float gunDepth = upperArmWidth;
float stanchionLength = robotBodyLength;
float stanchionRadius = 0.1 * robotBodyDepth;
float baseWidth = 2 * robotBodyWidth;
float baseLength = 0.25 * stanchionLength;
float upperLegLength = 0.6 * robotBodyLength;
float upperLegWidth = 0.08 * robotBodyWidth;
//control head
float headAngle = 0, headX = 0, headY = 0, headZ = 0;

// Control Robot body rotation on base
// Joint angles
float bodyAngle = 0.0;
float hipAngle = 0.0;
float rKneeAngle = 0.0;

// Animation control variables
float hipIncrement = 2.0f;
float kneeIncrement = 2.0f;
bool animate = false;
bool stepForward = true;
// Control arm rotation
float shoulderAngle = -40.0;
float gunAngle = -25.0;

// Spin Cube Mesh
float cubeAngle = 0.0;
int rightLegAngle = 0.0;
int canonRotate = 0.0;
bool spinning = false;

struct RobotConfig {
    float x, y, z;
    float scale;
};
struct Projectile {
    VECTOR3D position;
    VECTOR3D velocity;
    bool active;
};

// Define positions and scales for each robot
RobotConfig robots[3] = {
    {0.0, 10, -15, 0.5},  // First robot at z = -30
    {15.0, 10, -5, 0.5},  // Second robot at z = -35 and slightly smaller
    {-20.0, 10, -15, 0.5}, // Third robot at z = -40 and slightly larger
};

VECTOR3D canonTip[3]; // Stores the world coordinates of each robot's cannon tip
VECTOR3D robotPosition[3] = {
    VECTOR3D(0.0, 10.0, -15.0),  // First robot's position
    VECTOR3D(15.0, 10.0, -5.0),  // Second robot's position
    VECTOR3D(-20.0, 10.0, -15.0) // Third robot's position
};
float robotRotation[3] = { 0.0f, 0.0f, 0.0f }; // Assume no initial rotation for robots

// Lighting/shading and material properties for robot - upcoming lecture - just copy for now
// Robot RGBA material properties (NOTE: we will learn about this later in the semester)
GLfloat robotBody_mat_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat robotBody_mat_specular[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat robotBody_mat_diffuse[] = { 0.7f,0.6f,0.6f,1.0f };
GLfloat robotBody_mat_shininess[] = { 10.0F };

GLfloat pentagon_mat_ambient[] = { 0.6f, 0.6f, 0.6f, 1.0f };  // Lighter ambient
GLfloat pentagon_mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };  // Stronger diffuse
GLfloat pentagon_mat_specular[] = { 0.9f, 0.9f, 0.9f, 1.0f };
GLfloat pentagon_mat_shininess[] = { 32.0F };

GLfloat dark_grey_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };   // Darker ambient
GLfloat dark_grey_diffuse[] = { 0.3f, 0.3f, 0.3f, 1.0f };   // Darker diffuse
GLfloat dark_grey_specular[] = { 0.4f, 0.4f, 0.4f, 1.0f };  // Medium specular
GLfloat dark_grey_shininess[] = { 20.0F };  // Lower shininess for a matte effect


GLfloat robotArm_mat_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat robotArm_mat_diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat robotArm_mat_specular[] = { 0.7f, 0.6f, 0.6f, 1.0f };
GLfloat robotArm_mat_shininess[] = { 32.0F };

GLfloat gun_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat gun_mat_diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat gun_mat_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat gun_mat_shininess[] = { 100.0F };

GLfloat robotLowerBody_mat_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat robotLowerBody_mat_diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat robotLowerBody_mat_specular[] = { 0.774597f, 0.774597f, 0.774597f, 1.0f };
GLfloat robotLowerBody_mat_shininess[] = { 76.8F };


// Light properties
GLfloat light_position0[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat light_position1[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_ambient[] = { 0.2F, 0.2F, 0.2F, 1.0F };



// Mouse button
int currentButton;



// Structure defining a bounding box, currently unused
typedef struct BoundingBox {
    VECTOR3D min;
    VECTOR3D max;
} BBox;

// Prototypes for functions in this module
void initOpenGL(int w, int h);
void display(void);
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void mouseMotionHandler(int xMouse, int yMouse);
void keyboard(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);
void animationHandler(int param);
void drawRobot();
void drawRobots();
void drawStars();
void drawBody();
void drawHead();
void drawLowerBody();
void drawLeftArm();
void drawLeftLeg();
void drawRightLeg();
void fireTimer(int value);
//void drawCapsule(float x, float y);
void initProjectiles();
void fireProjectile();
void updateProjectiles();
void drawProjectiles();
void drawJaw();
void drawRightArm();
void drawLegs();
// Declaration at the top of your file, after includes
void drawCanonTip(int robotIndex);
void drawCanon();
void drawTextureCube();
void animateStep(int value);
void setupTextures();
//void setupTexturess();
void drawPlayerCanon();
void mouseMotion(int x, int y);
//void update();
void animateCanon(int value);
bool stop = false;


int main(int argc, char** argv)
{
    // Initialize GLUT
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(vWidth, vHeight);
    glutInitWindowPosition(200, 30);
    glutCreateWindow("3D Hierarchical Example");

    // Initialize GL
    initOpenGL(vWidth, vHeight);
    initProjectiles();
    // Register callback functions
    //glutMotionFunc(mouseMotion); // Motion with mouse button pressed
    glutPassiveMotionFunc(mouseMotion); // Motion without mouse button
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    //glutMouseFunc(mouse);
    //glutMotionFunc(mouseMotionHandler);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, animateCanon, 0);
    animate = true;  // Set animation flag to true
    glutTimerFunc(50, animateStep, 0); // Initialize animation timer
    glutSpecialFunc(functionKeys);
    glutTimerFunc(1000, fireTimer, 0);


    // Start event loop, never returns
    glutMainLoop();

    return 0;
}

void setupTextures() {
    RGBpixmap pix[6];
    readBMPFile(&pix[0], "C:/Users/User/Downloads/RoboGame-main/RoboGame-main/skin01.bmp");    setTexture(&pix[0], 2000);
    readBMPFile(&pix[1], "C:/Users/User/Downloads/RoboGame-main/RoboGame-main/skin01.bmp");  setTexture(&pix[1], 2001);
    readBMPFile(&pix[2], "C:/Users/User/Downloads/RoboGame-main/RoboGame-main/skin01.bmp");   setTexture(&pix[2], 2002);
    readBMPFile(&pix[3], "C:/Users/User/Downloads/RoboGame-main/RoboGame-main/skin01.bmp"); setTexture(&pix[3], 2003);
    readBMPFile(&pix[4], "C:/Users/User/Downloads/RoboGame-main/RoboGame-main/skin01.bmp");   setTexture(&pix[4], 2004);
    readBMPFile(&pix[5], "C:/Users/User/Downloads/RoboGame-main/RoboGame-main/skin01.bmp");  setTexture(&pix[5], 2005);
}
/*
void setupTexturess() {
    RGBpixmap pix[6];
    readBMPFile(&pix[0], "tiles01.bmp");    setTexture(&pix[0], 2000);
    readBMPFile(&pix[1], "tiles01.bmp");  setTexture(&pix[1], 2001);
    readBMPFile(&pix[2], "tiles01.bmp");   setTexture(&pix[2], 2002);
    readBMPFile(&pix[3], "tiles01.bmp"); setTexture(&pix[3], 2003);
    readBMPFile(&pix[4], "tiles01.bmp");   setTexture(&pix[4], 2004);
    readBMPFile(&pix[5], "tiles01.bmp");  setTexture(&pix[5], 2005);
}*/
// Set up OpenGL. For viewport and projection setup see reshape().
void initOpenGL(int w, int h)
{
    // Set up and enable lighting
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);   // This second light is currently off

    // Other OpenGL setup
    glEnable(GL_DEPTH_TEST);   // Remove hidded surfaces
    glEnable(GL_TEXTURE_2D);
    setupTextures();
    //setupTexturess();
    glShadeModel(GL_SMOOTH);   // Use smooth shading, makes boundaries between polygons harder to see
    // Set background to a dark blue sky (R=0, G=0, B=0.4)
    glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

    glClearDepth(1.0f);
    glEnable(GL_NORMALIZE);    // Renormalize normal vectors
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   // Nicer perspective

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}
void drawTextureCube() {
    glEnable(GL_TEXTURE_2D);

    // Front face
    glBindTexture(GL_TEXTURE_2D, 2005); // Front texture ID
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, -0.5, 0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.5, -0.5, 0.5);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.5, 0.5, 0.5);
    glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, 0.5, 0.5);
    glEnd();

    // Back face
    glBindTexture(GL_TEXTURE_2D, 2004); // Back texture ID
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, 0.5, -0.5);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.5, 0.5, -0.5);
    glTexCoord2f(0.0, 1.0); glVertex3f(0.5, -0.5, -0.5);
    glEnd();

    // Left face
    glBindTexture(GL_TEXTURE_2D, 2002); // Left texture ID
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, -0.5, 0.5);
    glTexCoord2f(1.0, 1.0); glVertex3f(-0.5, 0.5, 0.5);
    glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, 0.5, -0.5);
    glEnd();

    // Right face
    glBindTexture(GL_TEXTURE_2D, 2001); // Right texture ID
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.5, -0.5, 0.5);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.5, 0.5, 0.5);
    glTexCoord2f(0.0, 1.0); glVertex3f(0.5, 0.5, -0.5);
    glEnd();

    // Top face
    glBindTexture(GL_TEXTURE_2D, 2000); // Top texture ID
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, 0.5, -0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, 0.5, 0.5);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.5, 0.5, 0.5);
    glTexCoord2f(0.0, 1.0); glVertex3f(0.5, 0.5, -0.5);
    glEnd();

    // Bottom face
    glBindTexture(GL_TEXTURE_2D, 2003); // Bottom texture ID
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.5, -0.5, 0.5);
    glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, -0.5, 0.5);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void drawPlayerCanon()
    {
    int ang;
    int delang = 30;
    float r0 = 1;
    float x0 = r0;
    float x1 = r0;
    float z0 = 0.0;
    float z1 = 0.0;

    glRotatef(cannonAngleX, 1.0, 0.0, 0.0); // Rotate around X-axis
    glRotatef(cannonAngleY, 0.0, 1.0, 0.0); // Rotate around Y-axis
    glTranslatef(0.0, 7.5, 17);
    glRotatef(90, 0.0, 0.0, 1.0);
    glRotatef(90, 1.0, 0.0, 0.0);

    glBegin(GL_QUADS);
    for (ang = 0; ang <= 360; ang += delang)
    {
        x0 = r0 * cos((double)ang * 2.0 * pi / 360.0);
        x1 = r0 * cos((double)(ang + delang) * 2.0 * pi / 360.0);
        z0 = r0 * sin((double)ang * 2.0 * pi / 360.0);
        z1 = r0 * sin((double)(ang + delang) * 2.0 * pi / 360.0);
        glTexCoord2f((float)ang / 360.0, 0.0);
        glVertex3f(x0, 7.0, z0);
        glTexCoord2f((float)ang / 360.0, 1.0);
        glVertex3f(x0, 0.8, z0);
        glTexCoord2f((float)(ang + delang) / 360.0, 1.0);
        glVertex3f(x1, 0.8, z1);
        glTexCoord2f((float)(ang + delang) / 360.0, 0.0);
        glVertex3f(x1, 7.0, z1);
        x0 = x1;
        z0 = z1;
    }
    glEnd();
    }

void fireTimer(int value) {
    fireProjectile();               // Call to fire a projectile
    glutPostRedisplay();            // Request a redraw
    glutTimerFunc(1000, fireTimer, 0); // Re-register timer, adjust interval as needed
}

void animationHandler(int param) {
    if (animate) {
        // Randomly fire projectiles
        if (rand() % 10 < 2) { // Adjust probability as needed
            fireProjectile();
        }

        updateProjectiles(); // Move projectiles
        glutPostRedisplay(); // Redraw the scene
        glutTimerFunc(33, animationHandler, 0); // ~30 FPS
    }
}



#define MAX_PROJECTILES 10
Projectile projectiles[MAX_PROJECTILES];

void initProjectiles() {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        projectiles[i].active = false;
    }
}

// Define these at a global level or within an appropriate scope
float canonX = 0.0; // X position of the canon
float canonY = 0.0; // Y position of the canon
float canonZ = 0.0; // Z position of the canon tip
float canonAngle = 0.0; // Angle in radians
float speed = 1.0; // Speed of the projectile

void fireProjectile() {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectiles[i].active) {
            int robotIndex = rand() % 3; // Randomly select a robot

            projectiles[i].position = canonTip[robotIndex]; // Set starting position at cannon tip
            projectiles[i].velocity = VECTOR3D(0.0, 0.0, 1.0); // Move toward the camera
            projectiles[i].active = true;

            break; // Fire only one projectile
        }
    }
}




void updateProjectiles() {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (projectiles[i].active) {
            projectiles[i].position = projectiles[i].position + projectiles[i].velocity;

            // Deactivate projectile if out of bounds
            if (projectiles[i].position.z > 10 || projectiles[i].position.z < -50) {
                projectiles[i].active = false;
            }
        }
    }
}


void drawProjectiles() {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (projectiles[i].active) {
            glPushMatrix();
            glColor3f(1.0, 0.0, 0.0); // Red projectiles
            glTranslatef(projectiles[i].position.x, projectiles[i].position.y, projectiles[i].position.z);
            glutSolidSphere(0.5, 20, 20); // Adjust size as needed
            glPopMatrix();
        }
    }
}


void animateStep(int value)
{
    if (!animate)
        return;

    static bool forward = true; // Toggles the direction of movement

    // Increment or decrement angles for alternating leg motion
    if (forward) {
        hipAngle += 2.0f;   // Adjust this value for faster motion
        rKneeAngle -= 1.5f; // Larger bend increment for more pronounced motion

        // Reverse direction at smaller max angles
        if (hipAngle >= 15.0f || rKneeAngle <= -10.0f) {
            forward = false;
        }
    }
    else {
        hipAngle -= 2.0f;
        rKneeAngle += 1.5f;

        // Reset angles at smaller min angles
        if (hipAngle <= 0.0f && rKneeAngle >= 0.0f) {
            forward = true;

            // Move robots forward slightly after completing a step
            for (int i = 0; i < 3; i++) {
                robots[i].z += 0.5f; // Larger increment for faster movement
            }
        }
    }

    // Redraw the scene with updated positions
    glutPostRedisplay();
    glutTimerFunc(30, animateStep, 0); // Faster updates for smoother motion
}

void animateCanon(int value)
{
    if (spinning) {
        canonRotate += 2.0;

        if (canonRotate >= 360.0f) {
            canonRotate = 0.0f;

        }
        glutPostRedisplay();
        glutTimerFunc(16, animateCanon, 0);
    }

}

// Callback, called whenever GLUT determines that the window should be redisplayed
// or glutPostRedisplay() has been called.
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(LookX, LookY, LookZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    drawStars();
    
    // Draw the ground as a large green quad lower than the robot base
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST); // Temporarily disable depth testing for the ground
    glColor3f(0.0f, 0.5f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(-1000.0f, -1.0f, 1000.0f); // Lower the ground further if necessary
    glVertex3f(-1000.0f, -1.0f, -1000.0f);
    glVertex3f(1000.0f, -1.0f, -1000.0f);
    glVertex3f(1000.0f, -1.0f, 1000.0f);
    glEnd();
    glEnable(GL_DEPTH_TEST); // Re-enable depth testing
    glEnable(GL_LIGHTING);

    glColor3f(1.0f, 1.0f, 1.0f);

    // Continue with the rest of your scene
    drawRobots();
    drawPlayerCanon();
    updateProjectiles();  // Update positions before drawing
    drawProjectiles();  // Ensure this is actually being called
    glutSwapBuffers();
}



void drawRobots() {
    for (int i = 0; i < 3; i++) {
        glPushMatrix();  // Save the current state of transformations
        // Apply translation and scaling for each robot
        glTranslatef(robots[i].x, robots[i].y, robots[i].z);
        glScalef(robots[i].scale, robots[i].scale, robots[i].scale);
        drawRobot();  // Draw robot using existing function
        drawCanonTip(i);
        glPopMatrix();  // Restore the state of transformations
    }
}
void drawRobot()
{
    glPushMatrix();
    // Apply rotation for the robot's body (base)
    //glTranslatef(0.0, 5.0, 0.0); // Move the robot up by 9 units
    glRotatef(bodyAngle, 0.0, 0.0, 0.0);
    // Draw body
    drawBody();

    // Draw head
    drawHead();
    // Draw left leg with hip and knee rotations

    // Draw right leg with hip and knee rotations
    drawRightLeg();
    // Draw left arm
    drawLeftArm();
    // Draw right arm
    drawRightArm();
    //drawCanon();
    // Draw left leg with hip and knee rotations
    drawLeftLeg();

    glPopMatrix(); // End of the whole robot
}
void drawStars() {
    int numStars = 1000;  // Number of stars you want
    glPointSize(2.0);  // Set point size for stars

    glDisable(GL_LIGHTING);  // Disable lighting for drawing stars
    glColor3f(1.0, 1.0, 1.0);  // Set color to white

    glBegin(GL_POINTS);
    for (int i = 0; i < numStars; ++i) {
        float x = (rand() % 2000 - numStars) / 10.0;  // Random x between -100 and 100
        float y = (rand() % 2000 - numStars) / 10.0;  // Random y between -100 and 100
        float z = -((rand() % 200 + 50) / 10.0);  // Random z to make sure stars are in the background
        glVertex3f(x, y, z);
    }
    glEnd();

    glEnable(GL_LIGHTING);  // Re-enable lighting after drawing stars
}
void setupClippingPlane()
{
    GLdouble eqn[] = { 0.0, 1.0, 0.0, 0.0 };  // Clipping plane equation: y = 0
    glClipPlane(GL_CLIP_PLANE0, eqn);         // Define the clipping plane
    glEnable(GL_CLIP_PLANE0);                 // Enable the clipping plane
}


void drawFinger(float length, float width)
{
    // Set material properties for the pentagonal prism
    glMaterialfv(GL_FRONT, GL_AMBIENT, pentagon_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, pentagon_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, pentagon_mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, pentagon_mat_shininess);
    glPushMatrix();
    glScalef(width, length, width);
    drawTextureCube();
    glPopMatrix();
}

void drawTaperedPentagonalPrism(float topRadius, float bottomRadius, float height)
{
    // Set material properties for the pentagonal prism
    glMaterialfv(GL_FRONT, GL_AMBIENT, pentagon_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, pentagon_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, pentagon_mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, pentagon_mat_shininess);
    glPushMatrix();
    // Define the vertices for the top and bottom pentagons
    const int numVertices = 5;
    float angleIncrement = 2.0f * M_PI / numVertices;
    float topVertices[numVertices][2];
    float bottomVertices[numVertices][2];

    // Calculate the vertices for the top (narrower) pentagon
    for (int i = 0; i < numVertices; i++)
    {
        float angle = i * angleIncrement;
        topVertices[i][0] = topRadius * cos(angle);    // X-coordinate
        topVertices[i][1] = topRadius * sin(angle);    // Y-coordinate
    }

    // Calculate the vertices for the bottom (wider) pentagon
    for (int i = 0; i < numVertices; i++)
    {
        float angle = i * angleIncrement;
        bottomVertices[i][0] = bottomRadius * cos(angle);    // X-coordinate
        bottomVertices[i][1] = bottomRadius * sin(angle);    // Y-coordinate
    }

    // Draw the top face (narrower)
    glBegin(GL_POLYGON);
    for (int i = 0; i < numVertices; i++)
        glVertex3f(topVertices[i][0], height / 2, topVertices[i][1]);
    glEnd();

    // Draw the bottom face (wider)
    glBegin(GL_POLYGON);
    for (int i = 0; i < numVertices; i++)
        glVertex3f(bottomVertices[i][0], -height / 2, bottomVertices[i][1]);
    glEnd();

    // Draw the sides by connecting the top and bottom vertices
    glBegin(GL_QUADS);
    for (int i = 0; i < numVertices; i++)
    {
        int next = (i + 1) % numVertices;
        glVertex3f(topVertices[i][0], height / 2, topVertices[i][1]);          // Top vertex
        glVertex3f(topVertices[next][0], height / 2, topVertices[next][1]);    // Next top vertex
        glVertex3f(bottomVertices[next][0], -height / 2, bottomVertices[next][1]); // Next bottom vertex
        glVertex3f(bottomVertices[i][0], -height / 2, bottomVertices[i][1]);   // Bottom vertex
    }
    glEnd();

    glPopMatrix();
}




void drawCylinder(float radius, float height)
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, dark_grey_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dark_grey_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, dark_grey_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, dark_grey_shininess);

    glPushMatrix();

    // Bind the texture
    glEnable(GL_TEXTURE_2D); // Enable 2D texturing
    glBindTexture(GL_TEXTURE_2D, 2006); // Assume texture ID 2006 is assigned to tiles01.bmp

    // Draw the cylinder with texture coordinates
    GLUquadric* quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE); // Enable texturing for the quadric
    gluCylinder(quadric, radius, radius, height, 20, 20); // Draw the textured cylinder

    // Draw the bottom cap
    glPushMatrix();
    glRotatef(180, 1.0, 0.0, 0.0); // Flip to draw the bottom disk
    gluDisk(quadric, 0.0, radius, 20, 1);  // Bottom cap
    glPopMatrix();

    // Draw the top cap
    glPushMatrix();
    glTranslatef(0.0, 0.0, height); // Move to the top
    gluDisk(quadric, 0.0, radius, 20, 1);  // Top cap
    glPopMatrix();

    // Clean up
    gluDeleteQuadric(quadric);
    glDisable(GL_TEXTURE_2D); // Disable texturing
    glPopMatrix();
}

void drawCanon()
{
    glPushMatrix();
    glRotatef(canonRotate, 0, 0, 1);

    glPushMatrix();
    glScalef(2, 2, 20);
    glColor3f(0.7f, 0.2f, 0.5f);
    glutSolidTorus(0.3, 1.0, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    drawCylinder(1, 1);
    glPopMatrix();

    glPopMatrix();

}
void drawCanonTip(int robotIndex) {
    glPushMatrix();

    // Translate to the robot's position
    glTranslatef(robotPosition[robotIndex].x, robotPosition[robotIndex].y, robotPosition[robotIndex].z);

    // Apply rotation for the robot
    glRotatef(robotRotation[robotIndex], 0.0f, 1.0f, 0.0f);

    // Translate to the cannon's position relative to the robot
    glTranslatef(0.0, 4.5, -2.0); // Adjust based on where your cannon is

    // Get the world position of the cannon tip
    GLdouble modelview[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    canonTip[robotIndex] = VECTOR3D(modelview[12], modelview[13], modelview[14]);

    glPopMatrix();
}

void drawConnectingPole(float radius, float length)
{
    // Set material properties for the pentagonal prism
    glMaterialfv(GL_FRONT, GL_AMBIENT, pentagon_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, pentagon_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, pentagon_mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, pentagon_mat_shininess);
    glPushMatrix();
    GLUquadric* quadric = gluNewQuadric();
    gluCylinder(quadric, radius, radius, length, 20, 20);
    gluDeleteQuadric(quadric);
    glPopMatrix();
}
// Function to draw the upper and lower legs as cylinders
void drawLegCylinder(float radius, float height)
{

    glPushMatrix();
    GLUquadric* quadric = gluNewQuadric();
    gluCylinder(quadric, radius, radius, height, 20, 20);
    gluDeleteQuadric(quadric);
    glPopMatrix();
}

void drawCube(float len1, float width1, float len2, float width2, float depth)
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, pentagon_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, pentagon_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, pentagon_mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, pentagon_mat_shininess);
    //face 1

    glPushMatrix();
    //----------------------------------------------
    //close face
    float face1[4][2] =
    {
        { -len1, width1 }, //top left [0][0] [0][1]
        { len1, width1 }, //top right [1][0] [1][1]
        { len1, -width1 }, //bottom right [2][0] [2][1]
        { -len1, -width1 } //bottom left [3][0] [3][1]
    };

    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++)
    {
        glVertex3f(face1[i][0], face1[i][1], depth);
    }
    glEnd();
    //--------------------------------------------------
    //far face
    float face2[4][2] =
    {
        { -len2, width2 },
        { len2, width2 },
        { len2, -width2 },
        { -len2, -width2 }
    };
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++)
    {
        glVertex3f(face2[i][0], face2[i][1], -depth);
    }
    glEnd();
    //--------------------------------------------------
    //rest of faces
    glBegin(GL_QUADS);
    glVertex3f(face1[1][0], face1[1][1], depth);
    glVertex3f(face2[1][0], face2[1][1], -depth);
    glVertex3f(face2[2][0], face2[2][1], -depth);
    glVertex3f(face1[2][0], face1[2][1], depth);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(face1[0][0], face1[0][1], depth);
    glVertex3f(face2[0][0], face2[0][1], -depth);
    glVertex3f(face2[3][0], face2[3][1], -depth);
    glVertex3f(face1[3][0], face1[3][1], depth);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(face1[3][0], face1[3][1], depth);
    glVertex3f(face2[3][0], face2[3][1], -depth);
    glVertex3f(face2[2][0], face2[2][1], -depth);
    glVertex3f(face1[2][0], face1[2][1], depth);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(face1[0][0], face1[0][1], depth);
    glVertex3f(face2[0][0], face2[0][1], -depth);
    glVertex3f(face2[1][0], face2[1][1], -depth);
    glVertex3f(face1[1][0], face1[1][1], depth);
    glEnd();

    glEnd();



    glPopMatrix();
}

// Function to draw the foot as a scaled cube
void drawFoot(float length, float width, float depth)
{
    glPushMatrix();
    glScalef(width, depth, length);
    drawTextureCube();
    glPopMatrix();
}
void drawPentagonalPrism(float radius, float height)
{
    // Set material properties for the pentagonal prism
    glMaterialfv(GL_FRONT, GL_AMBIENT, pentagon_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, pentagon_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, pentagon_mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, pentagon_mat_shininess);
    glPushMatrix();
    // Define the vertices for a pentagon
    const int numVertices = 5;
    float angleIncrement = 2.0f * M_PI / numVertices;
    float vertices[numVertices][2];

    for (int i = 0; i < numVertices; i++)
    {
        float angle = i * angleIncrement;
        vertices[i][0] = radius * cos(angle);
        vertices[i][1] = radius * sin(angle);
    }

    // Draw the top face
    glBegin(GL_POLYGON);
    for (int i = 0; i < numVertices; i++)
        glVertex3f(vertices[i][0], height / 2, vertices[i][1]);
    glEnd();

    // Draw the bottom face
    glBegin(GL_POLYGON);
    for (int i = 0; i < numVertices; i++)
        glVertex3f(vertices[i][0], -height / 2, vertices[i][1]);
    glEnd();

    // Draw the sides
    glBegin(GL_QUADS);
    for (int i = 0; i < numVertices; i++)
    {
        int next = (i + 1) % numVertices;
        glVertex3f(vertices[i][0], height / 2, vertices[i][1]);
        glVertex3f(vertices[next][0], height / 2, vertices[next][1]);
        glVertex3f(vertices[next][0], -height / 2, vertices[next][1]);
        glVertex3f(vertices[i][0], -height / 2, vertices[i][1]);
    }
    glEnd();

    glPopMatrix();
}

void drawLeftLeg() {
    // Set material properties for the leg
    glMaterialfv(GL_FRONT, GL_AMBIENT, robotLowerBody_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, robotLowerBody_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, robotLowerBody_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, robotLowerBody_mat_shininess);

    // Start transformation
    glPushMatrix();

    // Translate to the hip joint position (where the leg connects to the body)
    glTranslatef(-4.0, -0.6 * robotBodyLength, 0.0);  // Translate to the hip joint position

    // Apply the rotation around the X-axis (for leg swinging forward/backward)
    glRotatef(hipAngle, 1.0, 0.0, 0.0);  // Rotate leg around the hip joint

    // Draw the hip joint (a sphere)
    glPushMatrix();
    glutSolidSphere(robotBodyWidth * 0.17, 20, 20);  // Hip joint sphere
    glPopMatrix();

    // Draw the upper leg (a cylinder)
    glPushMatrix();
    glTranslatef(0.0, -0.2 * robotBodyLength, 0.0);
    glRotatef(-90, 1.0, 0.0, 0.0);
    drawCylinder(robotBodyWidth * 0.12, robotBodyLength * 0.16);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, -0.8 * robotBodyLength, 0.0);
    glRotatef(-90, 1.0, 0.0, 0.0); // Align cylinder with the leg
    drawLegCylinder(robotBodyWidth * 0.08, robotBodyLength * 0.6); // Draw upper leg
    glPopMatrix();
    //-------------------------------------------------------------------------
       //lower leg
   //----------------------------------------------
    glPushMatrix();

    // Translate to the knee joint position
    glTranslatef(0.0, -0.8 * robotBodyLength, 0.0);

    // Apply the rotation around the knee joint (rKneeAngle is the variable for the knee rotation)
    glRotatef(rKneeAngle, 1.0, 0.0, 0.0);  // Rotate the lower leg around the knee

    // Draw the knee joint (a sphere)
    glPushMatrix();
    glutSolidSphere(robotBodyWidth * 0.12, 20, 20);  // Knee joint sphere
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, -0.3 * robotBodyLength, 0.0); // Start lower leg at the knee joint
    glRotatef(-90, 0.0, 1.0, 0.0); // Rotate 90 degrees around the y-axis to face forward


    // Draw the top tapered pentagonal prism
    drawTaperedPentagonalPrism(robotBodyWidth * 0.25, robotBodyWidth * 0.35, robotBodyLength * 0.4);
    // Move and draw the second pentagonal prism
    glTranslatef(0.0, -0.4 * robotBodyLength, 0.0); // Position the second prism below the first
    drawPentagonalPrism(robotBodyWidth * 0.35, robotBodyLength * 0.6);
    //8
    glPopMatrix(); // End of lower leg;

    // --- Foot ---
    //9
    glPushMatrix();
    glTranslatef(0.0, -1.2 * robotBodyLength, robotBodyWidth * 0.2); // Position the foot at the end of the lower leg
    drawFoot(robotBodyWidth * 0.99, robotBodyWidth * 0.72, robotBodyWidth * 0.2); // Draw the foot
    glPopMatrix();

    glPopMatrix();




    glPopMatrix(); // End lower leg transformation
    glPopMatrix();
}

void drawRightLeg() {
    // Set material properties for the leg
    glMaterialfv(GL_FRONT, GL_AMBIENT, robotLowerBody_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, robotLowerBody_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, robotLowerBody_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, robotLowerBody_mat_shininess);

    // Start transformation
    glPushMatrix();

    // Translate to the hip joint position (where the leg connects to the body)
    glTranslatef(4.0, -0.6 * robotBodyLength, 0.0);  // Translate to the hip joint position

    // Apply the rotation around the X-axis (for leg swinging forward/backward)
    glRotatef(-hipAngle, 1.0, 0.0, 0.0);  // Rotate leg around the hip joint

    // Draw the hip joint (a sphere)
    glPushMatrix();
    glutSolidSphere(robotBodyWidth * 0.17, 20, 20);  // Hip joint sphere
    glPopMatrix();

    // Draw the upper leg (a cylinder)
    glPushMatrix();
    glTranslatef(0.0, -0.2 * robotBodyLength, 0.0);
    glRotatef(-90, 1.0, 0.0, 0.0);
    drawCylinder(robotBodyWidth * 0.12, robotBodyLength * 0.16);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, -0.8 * robotBodyLength, 0.0);
    glRotatef(-90, 1.0, 0.0, 0.0); // Align cylinder with the leg
    drawLegCylinder(robotBodyWidth * 0.08, robotBodyLength * 0.6); // Draw upper leg
    glPopMatrix();
    //-------------------------------------------------------------------------
       //lower leg
   //----------------------------------------------
    glPushMatrix();

    // Translate to the knee joint position
    glTranslatef(0.0, -0.8 * robotBodyLength, 0.0);

    // Apply the rotation around the knee joint (rKneeAngle is the variable for the knee rotation)
    glRotatef(rKneeAngle, 1.0, 0.0, 0.0);  // Rotate the lower leg around the knee

    // Draw the knee joint (a sphere)
    glPushMatrix();
    glutSolidSphere(robotBodyWidth * 0.12, 20, 20);  // Knee joint sphere
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, -0.3 * robotBodyLength, 0.0); // Start lower leg at the knee joint
    glRotatef(-rKneeAngle, 0.0, 1.0, 0.0); // Rotate 90 degrees around the y-axis to face forward


    // Draw the top tapered pentagonal prism
    drawTaperedPentagonalPrism(robotBodyWidth * 0.25, robotBodyWidth * 0.35, robotBodyLength * 0.4);
    // Move and draw the second pentagonal prism
    glTranslatef(0.0, -0.4 * robotBodyLength, 0.0); // Position the second prism below the first
    drawPentagonalPrism(robotBodyWidth * 0.35, robotBodyLength * 0.6);
    //8
    glPopMatrix(); // End of lower leg;

    // --- Foot ---
    //9
    glPushMatrix();
    glTranslatef(0.0, -1.2 * robotBodyLength, robotBodyWidth * 0.2); // Position the foot at the end of the lower leg
    drawFoot(robotBodyWidth * 0.99, robotBodyWidth * 0.72, robotBodyWidth * 0.2); // Draw the foot
    glPopMatrix();

    glPopMatrix();




    glPopMatrix(); // End lower leg transformation
    //glPopMatrix();
}


void mouseMotion(int x, int y) {
    static int lastX = x;
    static int lastY = y;

    // Calculate the change in mouse position
    int deltaX = x - lastX;
    int deltaY = y - lastY;

    // Update cannon rotation angles based on mouse movement
    cannonAngleX += deltaY * mouseSensitivity; // Y-axis movement affects X rotation
    cannonAngleY += deltaX * mouseSensitivity; // X-axis movement affects Y rotation

    // Clamp angles to avoid excessive rotation
    if (cannonAngleX > 90.0f) cannonAngleX = 90.0f;
    if (cannonAngleX < -90.0f) cannonAngleX = -90.0f;

    lastX = x;
    lastY = y;

    glutPostRedisplay(); // Request to redraw the scene
}




void drawPalm(float length, float width, float depth)
{
    // Set material properties for the pentagonal prism
    glMaterialfv(GL_FRONT, GL_AMBIENT, pentagon_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, pentagon_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, pentagon_mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, pentagon_mat_shininess);
    glPushMatrix();
    glScalef(width, depth, length);
    drawTextureCube();
    glPopMatrix();
}

//second
void drawLeftArm() {
    glMaterialfv(GL_FRONT, GL_AMBIENT, robotArm_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, robotArm_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, robotArm_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, robotArm_mat_shininess);

    glPushMatrix();  // Start transformation for the arm

    // Translate to the shoulder joint position (relative to the body)
    glTranslatef(0.5 * robotBodyWidth + 0.5 * upperArmWidth, 4.5, 0.0);

    // Apply the shoulder rotation (shoulderAngle controls the rotation)
    glRotatef(shoulderAngle - 30, 1.0, 0.0, 0.0);  // Rotate around the X-axis for shoulder rotation
    glRotatef(-30, 0.0, 1.0, 0.0);            // Rotate the arm outward around the Y-axis
    glRotatef(10, 0.0, 0.0, 1.0);             // Add slight rotation around the Z-axis

    // --- Shoulder Joint (Sphere) ---
    glPushMatrix();
    glutSolidSphere(upperArmWidth * 0.99, 20, 20);  // Shoulder joint sphere
    glPopMatrix();

    //    // --- Add Cylinder Below Shoulder Joint ---
    glPushMatrix();
    glTranslatef(0.0, 0.5 * upperArmLength - 1.8 * upperArmWidth - 2.3, 0.0); // Position the cylinder directly below the shoulder sphere
    glRotatef(-90, 1.0, 0.0, 0.0); // Rotate the cylinder to stand vertically (along y-axis)
    drawCylinder(upperArmWidth * 0.8, upperArmLength * 0.21); // Adjust cylinder size and overlap
    glPopMatrix();

    // --- Upper Arm (Cylinder) ---
    glPushMatrix();
    glTranslatef(0.0, -1 * upperArmLength, 0.0);  // Position the cylinder below the shoulder joint
    glRotatef(-90, 1.0, 0.0, 0.0);                 // Rotate cylinder to align with Y-axis
    drawCylinder(upperArmWidth * 0.4, upperArmLength);  // Draw the upper arm as a cylinder
    glPopMatrix();

    // --- Elbow Joint (Sphere) ---
    glPushMatrix();
    glTranslatef(0.0, -upperArmLength, 0.0);  // Position at the end of the upper arm
    glutSolidSphere(upperArmWidth * 0.75, 20, 20);  // Elbow joint sphere
    glPopMatrix();

    // --- Lower Arm (Tapered Pentagonal Prisms) ---
    glPushMatrix();
    glTranslatef(0.0, -1.3 * upperArmLength, 0.0);  // Adjust position below the elbow joint

    // First tapered pentagonal prism
    drawTaperedPentagonalPrism(upperArmWidth * 1.1, upperArmWidth * 1.75, upperArmLength * 0.4);

    // Translate and draw the second pentagonal prism
    glTranslatef(0.0, -upperArmLength * 0.40, 0.0);
    drawTaperedPentagonalPrism(upperArmWidth * 1.75, upperArmWidth * 1.2, upperArmLength * 0.6);

    // --- Palm and Fingers ---
    glPushMatrix();
    glTranslatef(0.0, -upperArmLength * 0.43, 0.0);  // Position palm at the end of the lower arm
    glRotatef(90, 1.0, 0.0, 0.0);  // Rotate the palm to align correctly

    // Draw the palm
    drawPalm(upperArmWidth * 1.2, upperArmWidth * 1.50, upperArmWidth * 0.15);
    drawCanon();
    // Draw the canon
    int robotIndex = 0;
    drawCanonTip(robotIndex);


    // Draw the fingers
    // Finger 1
    glPushMatrix();
    glTranslatef(-upperArmWidth * 0.4, 0, upperArmWidth * 0.4);  // Adjust position for the first finger
    glRotatef(90, 1.0, 0.0, 0.0);
    drawFinger(upperArmLength * 0.5, upperArmWidth * 0.3);
    glPopMatrix();

    // Finger 2
    glPushMatrix();
    glTranslatef(0, 0, upperArmWidth * 0.4);  // Adjust position for the second finger
    glRotatef(90, 1.0, 0.0, 0.0);
    drawFinger(upperArmLength * 0.5, upperArmWidth * 0.3);
    glPopMatrix();

    // Finger 3
    glPushMatrix();
    glTranslatef(upperArmWidth * 0.4, 0, upperArmWidth * 0.4);  // Adjust position for the third finger
    glRotatef(90, 1.0, 0.0, 0.0);
    drawFinger(upperArmLength * 0.5, upperArmWidth * 0.3);
    glPopMatrix();

    // Thumb
    glPushMatrix();
    glTranslatef(upperArmWidth * 0.6, upperArmWidth * 0.3, -upperArmWidth * 0.3);  // Position thumb to point outward
    glRotatef(45, 0.0, 1.0, 0.0);  // Rotate to align like a thumb
    glRotatef(30, 0.0, 0.0, 1.0);  // Slight tilt for thumb
    drawFinger(upperArmLength * 0.15, upperArmWidth * 0.2);
    glPopMatrix();

    glPopMatrix();  // End palm and fingers

    glPopMatrix();  // End lower arm

    glPopMatrix();  // End the entire left arm transformation
}



void drawBody()
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);

    glPushMatrix();
    glTranslatef(0.0, 0.4, 0.0); // Move the body up by 2 units (adjust this value as needed)

    glScalef(robotBodyWidth, robotBodyLength, robotBodyDepth);
    drawTextureCube();
    glPopMatrix();
    // --- Add a small sphere at the bottom center of the body ---
    glPushMatrix();
    glTranslatef(0.0, -0.42 * robotBodyLength - 0.2 * robotBodyWidth, 0.0); // Position sphere at the bottom middle
    glutSolidSphere(robotBodyWidth * 0.2, 18, 18); // Adjust the size of the sphere
    // --- Add horizontal connecting poles ---
        // Left pole
    glPushMatrix();
    glTranslatef(-0.3 * robotBodyWidth * 1.5, 0.0, 0.0); // Move to the left side of the sphere
    glRotatef(90, 0.0, 1.0, 0.0); // Align the pole horizontally
    drawConnectingPole(robotBodyWidth * 0.06, robotBodyWidth * 0.4); // Draw the left connecting pole
    glPopMatrix();

    // Right pole
    glPushMatrix();
    glTranslatef(0.02 * robotBodyWidth * 1.5, 0.0, 0.0); // Move to the right side of the sphere
    glRotatef(90, 0.0, 1.0, 0.0); // Align the pole horizontally
    drawConnectingPole(robotBodyWidth * 0.06, robotBodyWidth * 0.4); // Draw the right connecting pole
    glPopMatrix();

    glPopMatrix(); // End of sphere and poles
}

void drawHead()
{
    // Set robot material properties per body part. Can have seperate material properties for each part
    glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);



    glPushMatrix();
    //---------------------------------------------------------------------------
    // Position head with respect to parent (body

    glTranslatef(0, 0.5 * robotBodyLength + headLength / 1.7, 0); // this will be done last


    //neck cylinder
    glPushMatrix();
    //-----------------------------------------------------------
    glRotatef(90, 1.0, 0.0, 0.0);
    drawCylinder(headWidth / 4, headWidth / 2);
    glPopMatrix();

    glRotatef(headAngle, headX, headY, headZ);
    //jaw
    //-----------------------------------
    drawJaw();

    glPushMatrix();
    //-----------------------------------------------------------
    glTranslatef(0.0, 0.4, 0.0);
    //head cylinder
    glPushMatrix();
    glRotatef(90, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, 0.0);
    drawCylinder(headWidth / 2, headWidth / 4);
    glPopMatrix();
    //Top of head half sphere
    glPushMatrix();
    glScalef(0.4 * robotBodyWidth / 2, 0.4 * robotBodyWidth / 2, 0.4 * robotBodyWidth / 2);
    setupClippingPlane();
    glutSolidSphere(1, 50, 50);
    glDisable(GL_CLIP_PLANE0);
    glPopMatrix();
    //-----------------------------------------------------------
    glPopMatrix();

    //--------------------------------------------------------------------------
    glPopMatrix();
}

void drawJaw()
{
    float jawX1 = 0.2; float jawX2 = 0.2; //up
    float jawY1 = 2; float jawY2 = 0.5;//down
    float jawDepth = 3;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, dark_grey_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dark_grey_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, dark_grey_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, dark_grey_shininess);
    //-----------------------------------------------------------
    //jaw right
    glPushMatrix();
    //glTranslatef();
    glScalef(0.38, 0.38, 0.38);
    glTranslatef(4.5, -1.0, 1.0);
    glRotatef(14, 1.0, 0.0, 0.0);
    //glRotatef(200, 1.0, 0.0, 0.0);
    //glTranslatef()
    drawCube(jawX1, jawY1, jawX1, jawY2, jawDepth);

    glPopMatrix();
    //-----------------------------------------------------------
    //mouth Piece
    glPushMatrix();
    //glScalef(0.48, 0.48, 0.48);
    glTranslatef(0, -0.5, 2);
    drawCube(1, 0.6, 1, 0.6, 0.1);

    glPopMatrix();

    //-----------------------------------------------------------
    //jaw left
    glPushMatrix();
    //glTranslatef();
    glScalef(0.38, 0.38, 0.38);
    glTranslatef(-4.5, -1.0, 1.0);
    glRotatef(14, 1.0, 0.0, 0.0);
    //glRotatef(200, 1.0, 0.0, 0.0);
    //glTranslatef()
    drawCube(jawX1, jawY1, jawX1, jawY2, jawDepth);

    glPopMatrix();
}



void drawLowerBody()
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, robotLowerBody_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, robotLowerBody_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, robotLowerBody_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, robotLowerBody_mat_shininess);

    glPushMatrix();
    // Position stanchion and base with respect to body
    glTranslatef(0, -1.5 * robotBodyLength, 0.0); // this will be done last

    glPopMatrix();
}


void drawRightArm() {
    glMaterialfv(GL_FRONT, GL_AMBIENT, robotArm_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, robotArm_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, robotArm_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, robotArm_mat_shininess);

    glPushMatrix();  // Start transformation for the arm

    // Translate to the shoulder joint position (relative to the body)
    glTranslatef(-1 * (0.5 * robotBodyWidth + 0.5 * upperArmWidth), 4.5, 0.0);

    // Apply the shoulder rotation (shoulderAngle controls the rotation)
    glRotatef(0, 1.0, 0.0, 0.0);  // Rotate around the X-axis for shoulder rotation
    glRotatef(-30, 0.0, 1.0, 0.0);            // Rotate the arm outward around the Y-axis
    glRotatef(-10, 0.0, 0.0, 1.0);             // Add slight rotation around the Z-axis

    // --- Shoulder Joint (Sphere) ---
    glPushMatrix();
    glutSolidSphere(upperArmWidth * 0.99, 20, 20);  // Shoulder joint sphere
    glPopMatrix();

    //    // --- Add Cylinder Below Shoulder Joint ---
    glPushMatrix();
    glTranslatef(0.0, 0.5 * upperArmLength - 1.8 * upperArmWidth - 2.3, 0.0); // Position the cylinder directly below the shoulder sphere
    glRotatef(-90, 1.0, 0.0, 0.0); // Rotate the cylinder to stand vertically (along y-axis)
    drawCylinder(upperArmWidth * 0.8, upperArmLength * 0.21); // Adjust cylinder size and overlap
    glPopMatrix();

    // --- Upper Arm (Cylinder) ---
    glPushMatrix();
    glTranslatef(0.0, -1 * upperArmLength, 0.0);  // Position the cylinder below the shoulder joint
    glRotatef(-90, 1.0, 0.0, 0.0);                 // Rotate cylinder to align with Y-axis
    drawCylinder(upperArmWidth * 0.4, upperArmLength);  // Draw the upper arm as a cylinder
    glPopMatrix();

    // --- Elbow Joint (Sphere) ---
    glPushMatrix();
    glTranslatef(0.0, -upperArmLength, 0.0);  // Position at the end of the upper arm
    glutSolidSphere(upperArmWidth * 0.75, 20, 20);  // Elbow joint sphere
    glPopMatrix();

    // --- Lower Arm (Tapered Pentagonal Prisms) ---
    glPushMatrix();
    glTranslatef(0.0, -1.3 * upperArmLength, 0.0);  // Adjust position below the elbow joint

    // First tapered pentagonal prism
    drawTaperedPentagonalPrism(upperArmWidth * 1.1, upperArmWidth * 1.75, upperArmLength * 0.4);

    // Translate and draw the second pentagonal t
    glTranslatef(0.0, -upperArmLength * 0.40, 0.0);
    drawTaperedPentagonalPrism(upperArmWidth * 1.75, upperArmWidth * 1.2, upperArmLength * 0.6);

    // --- Palm and Fingers ---
    glPushMatrix();
    glTranslatef(0.0, -upperArmLength * 0.43, 0.0);  // Position palm at the end of the lower arm
    glRotatef(90, 1.0, 0.0, 0.0);  // Rotate the palm to align correctly

    // Draw the palm
    drawPalm(upperArmWidth * 1.2, upperArmWidth * 1.50, upperArmWidth * 0.15);

    // Draw the canon
    //drawCanon();

    // Draw the fingers
    // Finger 1
    glPushMatrix();
    glTranslatef(-upperArmWidth * 0.4, 0, upperArmWidth * 0.4);  // Adjust position for the first finger
    glRotatef(90, 1.0, 0.0, 0.0);
    drawFinger(upperArmLength * 0.5, upperArmWidth * 0.3);
    glPopMatrix();

    // Finger 2
    glPushMatrix();
    glTranslatef(0, 0, upperArmWidth * 0.4);  // Adjust position for the second finger
    glRotatef(90, 1.0, 0.0, 0.0);
    drawFinger(upperArmLength * 0.5, upperArmWidth * 0.3);
    glPopMatrix();

    // Finger 3
    glPushMatrix();
    glTranslatef(upperArmWidth * 0.4, 0, upperArmWidth * 0.4);  // Adjust position for the third finger
    glRotatef(90, 1.0, 0.0, 0.0);
    drawFinger(upperArmLength * 0.5, upperArmWidth * 0.3);
    glPopMatrix();

    // Thumb
    glPushMatrix();
    glTranslatef(-upperArmWidth * 0.6, upperArmWidth * 0.3, -upperArmWidth * 0.3);  // Position thumb to point outward
    glRotatef(45, 0.0, 1.0, 0.0);  // Rotate to align like a thumb
    glRotatef(30, 0.0, 0.0, 1.0);  // Slight tilt for thumb
    drawFinger(upperArmLength * 0.15, upperArmWidth * 0.2);
    glPopMatrix();

    glPopMatrix();  // End palm and fingers

    glPopMatrix();  // End lower arm

    glPopMatrix();  // End the entire left arm transformation
}






// Callback, called at initialization and whenever user resizes the window.
void reshape(int w, int h)
{
    // Set up viewport, projection, then change to modelview matrix mode -
    // display function will then set up camera and do modeling transforms.
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, (GLdouble)w / h, 0.2, 40.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set up the camera at position (0, 6, 22) looking at the origin, up along positive y axis
    gluLookAt(LookX, LookY, LookZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

}

void restartGame() {
    // Reset robot positions to their initial positions
    robots[0].x = 0.0;    robots[0].y = 10.0; robots[0].z = -15.0; // First robot
    robots[1].x = 15.0;   robots[1].y = 10.0; robots[1].z = -5.0;  // Second robot
    robots[2].x = -20.0;  robots[2].y = 10.0; robots[2].z = -15.0; // Third robot

    // Reset other game state variables
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        projectiles[i].active = false; // Deactivate all projectiles
    }

    // Reset animation state
    animate = true;  // Enable animation if robots are moving
    spinning = false; // Stop any spinning (e.g., canon rotation)

    // Reset angles and rotation states if necessary
    bodyAngle = 0.0;
    hipAngle = 0.0;
    rKneeAngle = 0.0;
    shoulderAngle = -40.0;
    gunAngle = -25.0;

    // Reset robot movement or animation states if needed
    for (int i = 0; i < 3; i++) {
        robotRotation[i] = 0.0f; // Reset any robot rotation
    }

    // Reset camera position if necessary
    LookX = 0.0;
    LookY = 10.0;
    LookZ = 22.0;

    // Optionally restart any movement for the robots (e.g., make them start moving from the beginning)
    for (int i = 0; i < 3; i++) {
        robots[i].z += 0.5f; // Make robots start moving forward slightly after reset
    }

    glutPostRedisplay();  // Redraw the scene after resetting
}


// Callback, handles input from the keyboard, non-arrow keys
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'r':  // Restart the game when the 'R' key is pressed
    case 'R':
        restartGame();
        break;
    case 'h': // Increase hip angle
        hipAngle += 5.0;
        break;
    case 'H': // Decrease hip angle
        hipAngle -= 5.0;
        break;
    case 'k': // Increase knee angle
        rKneeAngle += 5.0;
        break;
    case 'K': // Decrease knee angle
        rKneeAngle -= 5.0;
        break;
    case 'b': // Increase body rotation
        bodyAngle += 5.0;
        break;
    case 'B': // Decrease body rotation
        bodyAngle -= 5.0;
        break;
    case 's': // Increase shoulder angle
        shoulderAngle += 5.0;
        break;
    case 'S': // Decrease shoulder angle
        shoulderAngle -= 5.0;
        break;
    case 'w': // Start animation
        animate = true;
        glutTimerFunc(50, animateStep, 0); // Start animation loop
        break;
    case 'W': // Reset angles
        animate = false;
        bodyAngle = 0.0;
        hipAngle = 0.0;
        rKneeAngle = 0.0;
        shoulderAngle = -40.0;
        break;
    case 'x':
        LookX += 1;
        break;
    case 'y':
        LookY += 1;
        break;
    case 'X':
        LookX -= 1;
        break;
    case 'Y':
        LookY -= 1;
        break;
    case 'z':
        LookZ += 1;
        break;
    case 'Z':
        LookZ -= 1;
        break;
    case 'c':
        spinning = true;
        glutTimerFunc(10, animateCanon, 0);
        break;
    case 'C':
        spinning = false;
        break;
    }
    glutPostRedisplay();   // Trigger a window redisplay
}


// Callback, handles input from the keyboard, function and arrow keys
void functionKeys(int key, int x, int y)
{
    // Help key
    if (key == GLUT_KEY_F1)
    {

    }
    // Do transformations with arrow keys
    //else if (...)   // GLUT_KEY_DOWN, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_LEFT
    //{
    //}

    glutPostRedisplay();   // Trigger a window redisplay
}





