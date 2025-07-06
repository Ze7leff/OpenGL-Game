/*
Your very first OpenGL program

NOTE: even though the suffix is .cpp, this is *not* a C++ program. This is structly a C program (as are all the examples in this course)
*/

#include <windows.h>
#include <gl/glut.h>
#include <math.h>
#include <stdio.h>

void initOpenGL(void);
void draw(void);


GLint windowWidth = 500;
GLint windowHeight = 250;
GLint viewportWidth = windowWidth;
GLint viewportHeight = windowHeight;

// This is a simple 2D OpenGL example
//
// Note that gluOrtho2D just calls glOrtho and creates a very thin view volume
// OpenGL doesn't really support 2D graphics - it's really 3D with the z value of all vertices set to 0.0
// By default, the camera is positioned on the z axis looking toward the origin. This means you don't really
// need to use gluLookAt to position the camera


// Boundaries of your world - openGL is never told about these boundaries - 
//                            it is only told about your view of the world using glOrtho2D

GLdouble worldLeft = -400.0;
GLdouble worldRight = 400.0;
GLdouble worldBottom = -200.0;
GLdouble worldTop = 200.0;

// Boundaries of world view (wv) window - also called the 2D clip window
// or clip rectangle
// Analogy: Like setting up a 2D`camera and lens
GLdouble wvLeft = -200.0,
wvRight = 200.0,
wvBottom = -100.0,
wvTop = 100.0;


// Some attributes of the Square Polygon
GLdouble red = 1.0;
GLdouble green = 0.0;
GLdouble blue = 0.0;

// We are going to place our square in the middle of our world
GLdouble sqCenterX = 0.0;
GLdouble sqCenterY = 0.0;
GLdouble sqLength = 50.0;




int main(int argc, char* argv[])
{
	printf("hello graphics world!\n");

	glutInit(&argc, (char**)argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Simple Square Drawing and Manipulating Program");

	glutDisplayFunc(draw);

	initOpenGL();

	glutMainLoop();

	return 0;
}


void initOpenGL(void)
{
	// Make the viewport 500 pixels by 250 pixels, starting at 0,0
	// In main() below, we used glut to create a window of the same size. 
	// This viewport covers the entire window. We could also create multiple viewports 
	// within the glutWindow
	glViewport(0, 0, viewportWidth, viewportHeight);

	// We first decide on a "world" coordinate system for our scene 
	// (for example -400 to 400 in x, -200 to 300 in y)
	// Our world coordinate system can be whatever you want it to be - for example, if you are modeling a classroom,
	// perhaps you would set the world coordinate system to 0 to 20 feet in x, 0 to 12 feet in y, and 0 to 30 feet in z
	// We then setup a viewing window (i.e. we decide what part of the world we want to view - like aiming a camera at the
	// world). In this example, we make the viewing window (or clip window) be -200 to 200 in x and -100 to 100 in y. 
	// This would mean that any objects we placed in the world that were outside of this range would be clipped away.
	// Later, we might change the view window to be some other view of our world (i.e. we move our camera and take another
	// picture). Also note: the parameters are left, right, bottom, top for glOrtho2D()
	// OpenGL has a projection matrix and a MODELVIEW matrix. We first set up the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);
	// First set it to the identity matrix
	glLoadIdentity();
	// This just calls glOrtho and creates a very thin view volume i.e. very narrow range of z values
	gluOrtho2D(wvLeft, wvRight, wvBottom, wvTop);

	// Now set up the MODELVIEW matrix
	glMatrixMode(GL_MODELVIEW);
	// Since this is a 2D application, we are not specifying the location of
	// a camera using the gluLoookAt() function. By default, the camera or eye
	// is at the origin looking down the negative z axis, and the z range is very very small 
	// so we call nothing here. We are also not yet doing any translates/rotates/scales of our square
	// by calling glTranslate(), glRotate() etc. In this simple example, we will be changing the 
	// position of the square by setting the coordinates of its vertices *explicitly* 
}


/////////////////////////////////////////////////////////////////////////////////////
//
//       The draw callback function - here is where we draw our scene
//       This function is called by OpenGL for us once at the beginning and
//       every time we call glutPostRedisplay(). We *never* call this function
//       directly
/////////////////////////////////////////////////////////////////////////////////////

void draw(void)
{
	GLdouble llx, lly, lrx, lry, urx, ury, ulx, uly;

	glClear(GL_COLOR_BUFFER_BIT);

	// Set color of objects
	glColor3f(red, green, blue);

	// Compute vertex positions of square using our global variable above
	// ll = lower left, lr = lower right
	// ur = upper right,ul = upper left
	llx = sqCenterX - sqLength / 2.0;
	lly = sqCenterY - sqLength / 2.0;
	lrx = sqCenterX + sqLength / 2.0;
	lry = sqCenterY - sqLength / 2.0;
	urx = sqCenterX + sqLength / 2.0;
	ury = sqCenterY + sqLength / 2.0;
	ulx = sqCenterX - sqLength / 2.0;
	uly = sqCenterY + sqLength / 2.0;

	// Set the positions of the vertices and tell OpenGL we want to draw a polygon consisting of 4 vertices
	glBegin(GL_POLYGON);
	glVertex3f(llx, lly, 0.0);
	glVertex3f(lrx, lry, 0.0);
	glVertex3f(urx, ury, 0.0);
	glVertex3f(ulx, ury, 0.0);
	glEnd();

	// Force the scene to be drawn
	// OpenGL will multiply the square's vertex positions, one at a time, by the PROJECTION matrix and the 
	// MODELVIEW matrix and will assign the specified color
	glFlush();

}






