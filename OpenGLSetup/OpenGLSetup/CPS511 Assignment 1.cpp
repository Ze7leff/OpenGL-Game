/*******************************************************************
		   Hierarchical Multi-Part Model Example
********************************************************************/
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


//#define GL_SILENCE_DEPRECATION
const int vWidth = 650;    // Viewport width in pixels
const int vHeight = 500;    // Viewport height in pixels

// Note how everything depends on robot body dimensions so that can scale entire robot proportionately
// just by changing robot body scale

//manipulate camera
float LookX = 20.0;
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
float kneeAngle = 0.0;

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

// A template cube mesh
CubeMesh* cubeMesh = createCubeMesh();

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
void drawBody();
void drawHead();
void drawLowerBody();
void drawLeftArm();
void drawLeftLeg();
void drawRightLeg();
//void drawCapsule(float x, float y);
void drawJaw();
void drawRightArm();
//void drawLegs();
void animateStep(int value);
//void update();
void animateCanon(int value);



int main(int argc, char** argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(vWidth, vHeight);
    glutInitWindowPosition(200, 30);
    glutCreateWindow("3D Hierarchical Example");

    // Initialize GL
    initOpenGL(vWidth, vHeight);

    // Register callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotionHandler);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, animateCanon, 0);
    glutTimerFunc(50, animateStep, 0); // Initialize animation timer
    glutSpecialFunc(functionKeys);

    // Start event loop, never returns
    glutMainLoop();

    return 0;
}


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





    glShadeModel(GL_SMOOTH);   // Use smooth shading, makes boundaries between polygons harder to see 
    glClearColor(1.0f, 0.65f, 0.0f, 0.0f);  // Set background to orange (R=1, G=0.65, B=0)
    glClearDepth(1.0f);
    glEnable(GL_NORMALIZE);    // Renormalize normal vectors 
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   // Nicer perspective

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    


}

void animateStep(int value)
{
    if (!animate)
        return;

    // Animate hip and knee joints for stepping
    if (stepForward)
    {
        hipAngle += hipIncrement;
        kneeAngle -= kneeIncrement;

        // Switch direction at max angles
        if (hipAngle >= 30.0 || kneeAngle <= -30.0)
            stepForward = false;
    }
    else
    {
        hipAngle -= hipIncrement;
        kneeAngle += kneeIncrement;

        // Stop when angles reset
        if (hipAngle <= 0.0 && kneeAngle >= 0.0)
        {
            hipAngle = 0.0;
            kneeAngle = 0.0;
            animate = false;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(50, animateStep, 0);
}

// Callback, called whenever GLUT determines that the window should be redisplayed
// or glutPostRedisplay() has been called.
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    // Create Viewing Matrix V
    // Set up the camera at position (0, 6, 22) looking at the origin, up along positive y axis
    gluLookAt(LookX, LookY, LookZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // Draw Robot

    // Apply modelling transformations M to move robot
    // Current transformation matrix is set to IV, where I is identity matrix
    // CTM = IV
    drawRobot();

    // Example of drawing a mesh (closed cube mesh)

    glutSwapBuffers();   // Double buffering, swap buffers
}

void drawRobot()
{
    glPushMatrix();
    // Apply rotation for the robot's body (base)
    glTranslatef(0.0, 5.0, 0.0); // Move the robot up by 9 units
    glRotatef(bodyAngle, 0.0, 1.0, 0.0);
    // Draw body
    drawBody();
    // Draw head
    drawHead();
    // Draw left leg with hip and knee rotations
    //drawLeftLeg();
    // Draw right leg with hip and knee rotations
    //drawRightLeg();
    // Draw left arm
    drawLeftArm();
    // Draw right arm
    drawRightArm();

    glPopMatrix(); // End of the whole robot
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
    glutSolidCube(1.0);
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
    GLUquadric* quadric = gluNewQuadric();
    gluCylinder(quadric, radius, radius, height, 20, 20); // Draw cylinder

    glPushMatrix();
    glRotatef(180, 1.0, 0.0, 0.0); // Flip to draw the bottom disk
    gluDisk(quadric, 0.0, radius, 20, 1);  // Bottom cap
    glPopMatrix();

    // Draw the top cap
    glPushMatrix();
    glTranslatef(0.0, 0.0, height); // Move to the top
    gluDisk(quadric, 0.0, radius, 20, 1);  // Top cap
    glPopMatrix();

    gluDeleteQuadric(quadric);
    glPopMatrix();
}
void drawCanon()
{
    glPushMatrix();
    glRotatef(canonRotate, 0, 0, 1);

    glPushMatrix();
    glScalef(2,2,20);
    glColor3f(0.7f, 0.2f, 0.5f);
    glutSolidTorus(0.3, 1.0, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glRotatef(90,1,0,0);
    drawCylinder(1,1);
    glPopMatrix();

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


    /*for (int i = 0; i < 4; i++)
    {
        int next = (i + 1) % 4;
        glVertex3f(face2[i][0], face1[i][1], depth);//ok
        glVertex3f(face1[i][0], face2[i][1], -depth);//ok
        glVertex3f(face1[next][0], face2[next][1], -depth);
        glVertex3f(face2[next][0], face2[next][1], depth);
    }*/
    glEnd();



    glPopMatrix();
}

// Function to draw the foot as a scaled cube
void drawFoot(float length, float width, float depth)
{
    glPushMatrix();
    glScalef(width, depth, length);
    glutSolidCube(1.0);
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

void drawLeftLeg()
{
    // Set material properties for the leg
    glMaterialfv(GL_FRONT, GL_AMBIENT, robotLowerBody_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, robotLowerBody_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, robotLowerBody_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, robotLowerBody_mat_shininess);


    
    glPushMatrix();

    glPushMatrix();
    glTranslatef(20, 0.6 * robotBodyLength, 0.0); // Position the sphere higher
    glutSolidSphere(robotBodyWidth * 0.17, 20, 20); // Sphere for the hip joint
    glPopMatrix();

    glPopMatrix();

}

/*void drawLeftLeg()
{
    // Set material properties for the leg
    glMaterialfv(GL_FRONT, GL_AMBIENT, robotLowerBody_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, robotLowerBody_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, robotLowerBody_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, robotLowerBody_mat_shininess);


    
    //1
    glPushMatrix();    
    glTranslatef(-0.5 * robotBodyWidth, -1.2 * robotBodyLength, 0.0);
    glRotatef(hipAngle, 1.0, 0.0, 0.0);
    //2
    glPushMatrix();
    // --- Position the leg with respect to the body ---
    glTranslatef(-0.06 * robotBodyWidth, -0.0 * robotBodyLength, 0.0); // Adjust the position to be higher
    // --- Hip Joint as a Sphere (on top of the upper leg) --- 
    //3
    glPushMatrix();
    glTranslatef(0.1, 0.6 * robotBodyLength, 0.0); // Position the sphere higher
    glutSolidSphere(robotBodyWidth * 0.17, 20, 20); // Sphere for the hip joint
    glPopMatrix();


    // --- Add a small cylinder below the hip joint ---
    //4
    glPushMatrix();
    glTranslatef(0.0, 0.34 * robotBodyLength, 0.0); // Position the cylinder just below the hip joint
    glRotatef(-90, 1.0, 0.0, 0.0); // Align the cylinder vertically
    drawCylinder(robotBodyWidth * 0.12, robotBodyLength * 0.16); // Adjust size of the cylinder
    glPopMatrix();
    
    //--- Long Cylinder --
    //5
    glPushMatrix();
    glRotatef(-90, 1.0, 0.0, 0.0); // Align cylinder with the leg
    drawLegCylinder(robotBodyWidth * 0.08, robotBodyLength * 0.6); // Draw upper leg
    glPopMatrix();

    // --- Knee Joint ---
    //6
    glPushMatrix();
    glTranslatef(0.0, -0.1 * robotBodyLength, 0.0); // Position at the end of the upper leg
    //glRotatef(kneeAngle, 1.0, 0.0, 0.0); // Rotate knee joint
    glutSolidSphere(robotBodyWidth * 0.12, 20, 20); // Sphere for the knee joint, adjust size if needed
    glPopMatrix();
    
    // --- Lower Leg as Two Pentagonal Prisms ---
    //7
    glPushMatrix();
    glTranslatef(0.0, -0.4 * robotBodyLength, 0.0); // Start lower leg at the knee joint
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
   

    glPopMatrix(); // End of left leg
}
*/
void drawRightLeg()
{
    // Set material properties for the leg
    glMaterialfv(GL_FRONT, GL_AMBIENT, robotLowerBody_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, robotLowerBody_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, robotLowerBody_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, robotLowerBody_mat_shininess);

    glPushMatrix();
    glTranslatef(0.5 * robotBodyWidth, -1.2 * robotBodyLength, 0.0);

    //glPushMatrix();

    // --- Hip Joint as a Sphere ---
    glPushMatrix();
    glRotatef(hipAngle, 0, 1, 0);
    glTranslatef(0.0, 0.6 * robotBodyLength, 0.0); // Position the sphere higher

    glutSolidSphere(robotBodyWidth * 0.17, 20, 20); // Sphere for the hip joint
    glPopMatrix();

    // --- Position the right leg with respect to the body ---
    glTranslatef(0.06 * robotBodyWidth, -0.0 * robotBodyLength, 0.0); // Adjust to right side

    // --- Add a small cylinder below the hip joint ---
    glPushMatrix();
    glTranslatef(0.0, 0.34 * robotBodyLength, 0.0); // Position the cylinder just below the hip joint
    glRotatef(-90, 1.0, 0.0, 0.0); // Align the cylinder vertically
    drawCylinder(robotBodyWidth * 0.12, robotBodyLength * 0.16); // Adjust size of the cylinder
    glPopMatrix();

    // --- Hip Joint as a Sphere ---
    glPushMatrix();
    glTranslatef(0.0, 0.6 * robotBodyLength, 0.0); // Position the sphere higher

    glutSolidSphere(robotBodyWidth * 0.17, 20, 20); // Sphere for the hip joint
    glPopMatrix();

    // --- Upper Leg as a Cylinder ---
    glPushMatrix();
    glRotatef(-90, 1.0, 0.0, 0.0); // Align cylinder with the leg
    drawLegCylinder(robotBodyWidth * 0.08, robotBodyLength * 0.6); // Draw upper leg
    glPopMatrix();

    // --- Knee Joint ---
    glPushMatrix();
    glTranslatef(0.0, -0.1 * robotBodyLength, 0.0); // Position at the end of the upper leg
    glRotatef(kneeAngle, 1.0, 0.0, 0.0); // Rotate knee joint

    glutSolidSphere(robotBodyWidth * 0.12, 20, 20); // Sphere for the knee joint
    glPopMatrix();

    // --- Lower Leg as Two Pentagonal Prisms ---
    glPushMatrix();
    glTranslatef(0.0, -0.4 * robotBodyLength, 0.0); // Start lower leg at the knee joint
    glRotatef(-90, 0.0, 1.0, 0.0); // Rotate 90 degrees around the y-axis to face forward

    // Draw the top tapered pentagonal prism
    drawTaperedPentagonalPrism(robotBodyWidth * 0.25, robotBodyWidth * 0.35, robotBodyLength * 0.4);

    // Move and draw the second pentagonal prism
    glTranslatef(0.0, -0.4 * robotBodyLength, 0.0); // Position the second prism below the first
    drawPentagonalPrism(robotBodyWidth * 0.35, robotBodyLength * 0.6);

    glPopMatrix(); // End of lower leg

    // --- Foot ---
    glPushMatrix();
    glTranslatef(0.0, -1.2 * robotBodyLength, robotBodyWidth * 0.2); // Position the foot at the end of the lower leg
    drawFoot(robotBodyWidth * 0.99, robotBodyWidth * 0.72, robotBodyWidth * 0.2); // Draw the foot
    glPopMatrix();

    glPopMatrix();

    glPopMatrix(); // End of right leg
}

/*void drawLegs()
{
    //drawLeftLeg();
    drawRightLeg(); // Draw the right leg
}*/



void drawPalm(float length, float width, float depth)
{
    // Set material properties for the pentagonal prism
    glMaterialfv(GL_FRONT, GL_AMBIENT, pentagon_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, pentagon_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, pentagon_mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, pentagon_mat_shininess);
    glPushMatrix();
    glScalef(width, depth, length);
    glutSolidCube(1.0);
    glPopMatrix();
}

void drawLeftArm()
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, robotArm_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, robotArm_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, robotArm_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, robotArm_mat_shininess);

    glPushMatrix();
    glRotatef(10, 1, 0, 0);
    glPushMatrix();

    // Position arm with respect to parent body
    glTranslatef(0.5 * robotBodyWidth + 0.5 * upperArmWidth, 2.0, 0.0);
    glRotatef(-30, 0.0, 1.0, 0.0); // Rotate the arm 30 degrees outward around the y-axis
    glRotatef(10, 0.0, 0.0, 1.0);  // Add a slight rotation around the z-axis
    glRotatef(shoulderAngle, 1.0, 0.0, 0.0);

    // --- Shoulder Joint ---
    glPushMatrix();
    glTranslatef(0.0, 0.5 * upperArmLength, 0.0);
    glutSolidSphere(upperArmWidth * 0.99, 20, 20); // Use a sphere for the shoulder joint
    glPopMatrix();

    // --- Add Cylinder Below Shoulder Joint ---
    glPushMatrix();
    glTranslatef(0.0, 0.5 * upperArmLength - 1.8 * upperArmWidth, 0.0); // Position the cylinder directly below the shoulder sphere
    glRotatef(-90, 1.0, 0.0, 0.0); // Rotate the cylinder to stand vertically (along y-axis)
    drawCylinder(upperArmWidth * 0.8, upperArmLength * 0.23); // Adjust cylinder size and overlap
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0, 0.5 * upperArmLength - 2.5 * upperArmWidth, 0.0); // Position the cylinder directly below the shoulder sphere
    glRotatef(-90, 1.0, 0.0, 0.0); // Rotate the cylinder to stand vertically (along y-axis)
    drawCylinder(upperArmWidth * 0.7, upperArmLength * 0.13); // Adjust cylinder size and overlap
    glPopMatrix();

    // --- Replace Upper Arm with Cylinder ---
    glPushMatrix();
    glTranslatef(0.0, -3.0, 0.0);  // Ensure correct position for the cylinder
    glRotatef(-90, 1.0, 0.0, 0.0);  // Rotate cylinder along the correct axis
    drawCylinder(upperArmWidth * 0.5, upperArmLength);  // Adjust cylinder radius and length
    glPopMatrix();

    // --- Elbow Joint ---
    glPushMatrix();
    glTranslatef(0.0, -0.55 * upperArmLength, 0.0); // Position at the end of the upper arm
    glutSolidSphere(upperArmWidth * 0.75, 20, 20); // Smaller sphere for the elbow joint
    glPopMatrix();

    // --- Lower Arm as Two Pentagonal Prisms ---
    glPushMatrix();
    glTranslatef(0.0, -0.65 * upperArmLength - upperArmWidth, 0.0); // Adjust to eliminate space

    // Draw the first pentagonal prism
    drawTaperedPentagonalPrism(upperArmWidth * 1.1, upperArmWidth * 1.75, upperArmLength * 0.4);

    // Move and draw the second pentagonal prism
    glTranslatef(0.0, -upperArmLength * 0.40, 0.0);
    drawTaperedPentagonalPrism(upperArmWidth * 1.75, upperArmWidth * 1.2, upperArmLength * 0.6);

    // --- Add Palm at the End of the Lower Arm ---
    glPushMatrix();
    glTranslatef(0.0, -upperArmLength * 0.43, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0); // Rotate the palm to be vertical
    drawPalm(upperArmWidth * 1.2, upperArmWidth * 1.50, upperArmWidth * 0.15);

    drawCanon();
    // --- Add Fingers to the Palm ---
    // Finger 1
    glPushMatrix();
    glTranslatef(-upperArmWidth * 0.4, 0, upperArmWidth * 0.4); // Adjust position for the first finger
    glRotatef(90, 1.0, 0.0, 0.0);
    drawFinger(upperArmLength * 0.5, upperArmWidth * 0.3);
    glPopMatrix();

    // Finger 2
    glPushMatrix();
    glTranslatef(0, 0, upperArmWidth * 0.4); // Adjust position for the second finger
    glRotatef(90, 1.0, 0.0, 0.0);
    drawFinger(upperArmLength * 0.5, upperArmWidth * 0.3);
    glPopMatrix();

    // Finger 3
    glPushMatrix();
    glTranslatef(upperArmWidth * 0.4, 0, upperArmWidth * 0.4); // Adjust position for the third finger
    glRotatef(90, 1.0, 0.0, 0.0);
    drawFinger(upperArmLength * 0.5, upperArmWidth * 0.3);
    glPopMatrix();

    // Thumb
    glPushMatrix();
    glTranslatef(-upperArmWidth * 0.6, upperArmWidth * 0.3, -upperArmWidth * 0.3); // Position thumb to point outward
    glRotatef(45, 0.0, 1.0, 0.0);
    glRotatef(30, 0.0, 0.0, 1.0); // Slightly rotate to look like a thumb
    drawFinger(upperArmLength * 0.15, upperArmWidth * 0.2);
    glPopMatrix();

    glPopMatrix(); // End of palm and fingers

    glPopMatrix(); // End of lower arm

    glPopMatrix();

    glPopMatrix(); // End of entire left arm
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
    glutSolidCube(1.0);
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
    glRotatef(headAngle,headX,headY,headZ);
    glTranslatef(0,  0.5 *robotBodyLength +  headLength/1.7, 0); // this will be done last


    //neck cylinder
    glPushMatrix();
    //-----------------------------------------------------------
    glRotatef(90, 1.0, 0.0, 0.0);
    drawCylinder(headWidth / 4, headWidth / 2);
    glPopMatrix();

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
    drawCylinder(headWidth/2 , headWidth/4 );
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
    glScalef(0.38,0.38,0.38);
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





void drawRightArm()
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, robotArm_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, robotArm_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, robotArm_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, robotArm_mat_shininess);

    glPushMatrix();

    // Position arm with respect to parent body (right side)
    glTranslatef(-0.5 * robotBodyWidth - 0.5 * upperArmWidth, 2.0, 0.0);
    glRotatef(30, 0.0, 1.0, 0.0); // Rotate the arm 30 degrees outward around the y-axis
    glRotatef(-10, 0.0, 0.0, 1.0);  // Add a slight rotation around the z-axis

    // --- Shoulder Joint ---
    glPushMatrix();
    glTranslatef(0.0, 0.5 * upperArmLength, 0.0);
    glutSolidSphere(upperArmWidth * 0.99, 20, 20); // Use a sphere for the shoulder joint
    glPopMatrix();

    // --- Add Cylinder Below Shoulder Joint ---
    glPushMatrix();
    glTranslatef(0.0, 0.5 * upperArmLength - 1.8 * upperArmWidth, 0.0); // Position the cylinder directly below the shoulder sphere
    glRotatef(-90, 1.0, 0.0, 0.0); // Rotate the cylinder to stand vertically (along y-axis)
    drawCylinder(upperArmWidth * 0.8, upperArmLength * 0.23); // Adjust cylinder size and overlap
    glPopMatrix();

    // --- Replace Upper Arm with Cylinder ---
    glPushMatrix();
    glTranslatef(0.0, -3.0, 0.0);  // Ensure correct position for the cylinder
    glRotatef(-90, 1.0, 0.0, 0.0);  // Rotate cylinder along the correct axis
    drawCylinder(upperArmWidth * 0.5, upperArmLength);  // Adjust cylinder radius and length
    glPopMatrix();

    // --- Elbow Joint ---
    glPushMatrix();
    glTranslatef(0.0, -0.55 * upperArmLength, 0.0); // Position at the end of the upper arm
    glutSolidSphere(upperArmWidth * 0.75, 20, 20); // Smaller sphere for the elbow joint
    glPopMatrix();

    // --- Lower Arm as Two Pentagonal Prisms ---
    glPushMatrix();
    glTranslatef(0.0, -0.65 * upperArmLength - upperArmWidth, 0.0); // Adjust to eliminate space

    // Draw the first pentagonal prism
    drawTaperedPentagonalPrism(upperArmWidth * 1.1, upperArmWidth * 1.75, upperArmLength * 0.4);

    // Move and draw the second pentagonal prism
    glTranslatef(0.0, -upperArmLength * 0.40, 0.0);
    drawTaperedPentagonalPrism(upperArmWidth * 1.75, upperArmWidth * 1.2, upperArmLength * 0.6);

    // --- Add Palm at the End of the Lower Arm ---
    glPushMatrix();
    glTranslatef(0.0, -upperArmLength * 0.43, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0); // Rotate the palm to be vertical
    drawPalm(upperArmWidth * 1.2, upperArmWidth * 1.50, upperArmWidth * 0.15);

    
    // Add Fingers and Thumb as in left arm
    // Add similar code here for fingers and thumb

    glPopMatrix(); // End of palm and fingers
    glPopMatrix(); // End of lower arm
    glPopMatrix(); // End of right arm
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
bool stop = false;

// Callback, handles input from the keyboard, non-arrow keys
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'h': // Increase hip angle
        hipAngle += 5.0;
        break;
    case 'H': // Decrease hip angle
        hipAngle -= 5.0;
        break;
    case 'k': // Increase knee angle
        kneeAngle += 5.0;
        break;
    case 'K': // Decrease knee angle
        kneeAngle -= 5.0;
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
        kneeAngle = 0.0;
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



void animationHandler(int param)
{
    if (!stop)
    {
        shoulderAngle += 1.0;
        cubeAngle += 2.0;
        glutPostRedisplay();
        glutTimerFunc(10, animationHandler, 0);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                                                                        //Junk code
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------




/*ric = gluNewQuadric();

    // Draw the cylindrical side surface
    gluCylinder(quadric, radius, radius, height, 20, 20); // Side of cylinder

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

    gluDeleteQuadric(quadric);
    glPopMatrix();
}*/


/*void update(int value) {
    glutPostRedisplay();  // Redraw the scene
    glutTimerFunc(16, update, 0);  // Schedule the next update (60 FPS)
}*/


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


// Mouse button callback - use only if you want to 
void mouse(int button, int state, int x, int y)
{
    currentButton = button;

    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)
        {
            ;

        }
        break;
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN)
        {
            ;
        }
        break;
    default:
        break;
    }

    glutPostRedisplay();   // Trigger a window redisplay
}


// Mouse motion callback - use only if you want to 
void mouseMotionHandler(int xMouse, int yMouse)
{
    if (currentButton == GLUT_LEFT_BUTTON)
    {
        ;
    }

    glutPostRedisplay();   // Trigger a window redisplay
}


