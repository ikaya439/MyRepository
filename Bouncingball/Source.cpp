#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <math.h>
#include <windows.h>
#include <ctime>
#include "RgbImage.h"
#define PI 3.14159265f
int refreshMillis = 4;
int windowWidth = 800;
int windowHeight = 400;
int sure = 0;
int geciciSure = 0;
int timer = 5000;
bool ciz = true;
float startPointX = 3, startPointY = 5, startPointZ = -3;
static float locationX, locationY;
bool yuksel = false;
float limit = 5;
float speed = 0;
char* filename1 = "./ball.bmp";
char* filename2 = "./floor.bmp";
GLuint	texture[4];



void initGL() {
	glEnable(GL_DEPTH_TEST);
	glClearColor(138 / 255.0, 237 / 255.00, 237 / 255.0, 0.0);
	glShadeModel(GL_SMOOTH);
}
void loadTextureFromFile(char *filename, int i)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	RgbImage theTexMap(filename);

	// Pixel alignment: each row is word aligned (aligned to a 4 byte boundary)
	//    Therefore, no need to call glPixelStore( GL_UNPACK_ALIGNMENT, ... );


	glGenTextures(1, &texture[i]);					// Create The Texture
	glBindTexture(GL_TEXTURE_2D, texture[i]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// Typical Texture Generation Using Data From The Bitmap

	glTexImage2D(GL_TEXTURE_2D, 0, 3, theTexMap.GetNumCols(), theTexMap.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE, theTexMap.ImageData());
}
void zemin(){
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	float zeminNoktalari = 3;
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(2 * -zeminNoktalari, 0, zeminNoktalari + 4);
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(2 * -zeminNoktalari, 0, -zeminNoktalari);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(2 * zeminNoktalari, 0, -zeminNoktalari);
	glTexCoord2f(0.0f, 1.0f);	glVertex3f(2 * zeminNoktalari, 0, zeminNoktalari + 4);
	glEnd();
	glPopMatrix();
}
void ball(){
	GLUquadric *quad;
	quad = gluNewQuadric();
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	float boyut = 0.5f;
	glScalef(boyut, boyut, boyut);
	glTranslatef(startPointX, startPointY, startPointZ);
	gluQuadricTexture(quad, 1);
	gluSphere(quad, 1, 16, 16);
}
void hareket(){
	if (startPointY < 0.4){
		yuksel = true;
	}
	if (startPointY >= limit){
		yuksel = false;

	}
	if (yuksel){
		speed -= 0.00001;
		startPointY += abs(speed);
	}
	if (!yuksel){
		speed += 0.00001;
		startPointY -= abs(speed);
	}
	startPointX -= 0.003;
	startPointZ += 0.003;
	printf("%f %f %f\n", speed, limit, startPointY);
	glutPostRedisplay();
}
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLfloat transform_matrix[16];
	glLoadIdentity();
	gluLookAt(0, 5, 10, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	zemin();
	ball();
	hareket();
	glutSwapBuffers();
}
void reshape(int x, int y)
{
	if (y == 0 || x == 0) return;  //Nothing is visible then, so return
	//Set a new projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Angle of view:40 degrees
	//Near clipping plane distance: 0.5
	//Far clipping plane distance: 20.0
	gluPerspective(60.0, (GLdouble)x / (GLdouble)y, 0.5, 30.0);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, x, y);  //Use the whole window for rendering
}
void idle(){
	glutPostRedisplay();
}
void Timer(int value) {
	glutPostRedisplay(); // Post a paint request to activate display()
	glutTimerFunc(refreshMillis, Timer, 0); // subsequent timer call at milliseconds
}
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // ESC key
		exit(0);
		break;
	}
}
void specialKeys(int key, int x, int y) {

}
float uzaklikHesapla(float x, float y, float x1, float y1){
	return sqrt((pow(x - x1, 2) + pow(y - y1, 2)));
}

void mouse(int button, int state, int x, int y) {
	switch (state)
	{
	case GLUT_LEFT_BUTTON:
		glutSwapBuffers();

		break;
	default:
		break;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv); // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
	glutInitWindowSize(windowWidth, windowHeight); // Initial window width and height
	glutInitWindowPosition(200, 200); // Initial window top-left corner (x, y)
	glutCreateWindow("AraSinav2"); // Create window with given title
	glutDisplayFunc(display); // Register callback handler for window re-paint
	glutReshapeFunc(reshape); // Register callback handler for window re-shape
	//glutTimerFunc(0, Timer, 0); // First timer call immediately
	//glutIdleFunc(idle);
	//glutSpecialFunc(specialKeys); // Register callback handler for special-key event
	loadTextureFromFile(filename1, 1);
	loadTextureFromFile(filename2, 2);
	glutKeyboardFunc(keyboard); // Register callback handler for special-key event
	//	glutFullScreen(); // Put into full screen
	//glutFullScreen();
	glutMouseFunc(mouse); // Register callback handler for mouse event
	initGL(); // Our own OpenGL initialization
	glutMainLoop(); // Enter event-processing loop
	return 0;
}