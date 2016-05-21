#include <stdlib.h>
#include <glut.h>
#include "RgbImage.h"
GLfloat xRotated, yRotated, zRotated;
GLuint	texture[3];			// Storage For One Texture ( NEW )

/*
* Read a texture map from a BMP bitmap file.
*/
static float year = 0.0, day = 0;
static float cameraX = 0, aci = 0, cameraZ = 0, cameraY = 0, spinSun = 0, rotateCamera = 0;

static float px = 0, pz = 10, lx = 0, lz = 5, angle = 0;
static bool baslangic = true;
static float im = 0;//ilerleme miktarý


static int gecici = 0;
float cisimUzaklik = 12;

static int temp = 0;
static GLint fogMode;

//yeni
int i, j;
// angle of rotation for the camera direction

static struct asf{
	float speed;
	float boyut;
	float sunUzaklik;
	float planetSpin;
	float geciciplanetSpin;
}planetsArray[3];
void init(void){
	//glClearColor(1.0,1.0,1.0,0.0);
	glEnable(GL_DEPTH_TEST);
	//glClearColor(138 / 255.0, 237 / 255.00, 237 / 255.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	cameraZ = cisimUzaklik;
	planetsArray[0].boyut = 0.3;
	planetsArray[1].boyut = 0.4;
	planetsArray[2].boyut = 0.5;
	planetsArray[0].sunUzaklik = 1.4;
	planetsArray[1].sunUzaklik = 2.4;
	planetsArray[2].sunUzaklik = 3.4;
	planetsArray[0].planetSpin = 0.08;
	planetsArray[1].planetSpin = 0.04;
	planetsArray[2].planetSpin = 0.02;
	planetsArray[0].geciciplanetSpin = 0.08;
	planetsArray[1].geciciplanetSpin = 0.04;
	planetsArray[2].geciciplanetSpin = 0.02;
	planetsArray[0].speed = 0.0;
	planetsArray[1].speed = 0.0;
	planetsArray[2].speed = 0.0;
	//glEnable(GL_FOG);

	//{
	//	GLfloat fogColor[4] = { 0.5, 0.5, 0.5, 1.0 };
	//	fogMode = GL_LINEAR;
	//	glFogi(GL_FOG_MODE, fogMode);
	//	glFogfv(GL_FOG_COLOR, fogColor);
	//	glFogf(GL_FOG_DENSITY, 0.04);
	//	glHint(GL_FOG_HINT, GL_DONT_CARE);
	//	glFogf(GL_FOG_START, 0.0);
	//	glFogf(GL_FOG_END, 20.0);
	//}

}
void sun(float x, float y, float z, float rotateX, float rotateY, float rotateZ, float dimension){
	GLUquadric *quad;
	quad = gluNewQuadric();
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTranslatef(x, y, z);
	glColor3f(0.5, 0.5, 0.0);
	glRotatef((GLfloat)spinSun, rotateX, rotateY, rotateZ);
	gluQuadricTexture(quad, 1);
	gluSphere(quad, dimension, 20, 16);
	glPopMatrix();
}
void lightSourceAndSun(){

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light1_diffuse[] = { 1, 1, 1, 10 };
	GLfloat position[] = { 0.0, 2.0, -0.3, 1.0 };

	glLightfv(GL_LIGHT0, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glDisable(GL_LIGHTING);
	sun(0.0, 2.0, -0.3, 0.0, 1.0, 0.0, 1.0);
	glEnable(GL_LIGHTING);
}
void planetArrayOlustur(){
	GLfloat mat_diffuse[] = { 0.0, 0.0, 1.0, 1.0 };
	for (int i = 0; i < 3; i++){
		GLUquadric *quad;
		quad = gluNewQuadric();

		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		glTranslatef(0.0, 2.0, -0.1);
		planetsArray[i].speed += planetsArray[i].planetSpin;
		glRotatef((GLfloat)planetsArray[i].speed, 0.0, 1.0, 0.0);
		glTranslatef(planetsArray[i].sunUzaklik, 0.0, 0.0);
		glRotatef((GLfloat)planetsArray[i].speed, 0.0, 1.0, 0.0);
		glPushMatrix();
		gluQuadricTexture(quad, 1);
		gluSphere(quad, planetsArray[i].boyut, 20, 16);
		glPopMatrix();
		glPopMatrix();
		mat_diffuse[i] += 0.7;
	}
	glutPostRedisplay();
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
/*
* Draw the texture in the OpenGL graphics window
*/
void oda(){
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	float skyBox = 10.0f;
	glBegin(GL_QUADS);
	// Front Face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-skyBox, -skyBox, skyBox);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(skyBox, -skyBox, skyBox);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(skyBox, skyBox, skyBox);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-skyBox, skyBox, skyBox);
	glEnd();
	// Back Face
	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-skyBox, -skyBox, -skyBox);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-skyBox, skyBox, -skyBox);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(skyBox, skyBox, -skyBox);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(skyBox, -skyBox, -skyBox);
	glEnd();
	// Top Face
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-skyBox, skyBox, -skyBox);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-skyBox, skyBox, skyBox);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(skyBox, skyBox, skyBox);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(skyBox, skyBox, -skyBox);
	glEnd();
	// Bottom Face
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-skyBox, -skyBox, -skyBox);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(skyBox, -skyBox, -skyBox);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(skyBox, -skyBox, skyBox);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-skyBox, -skyBox, skyBox);
	glEnd();
	// Right face
	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(skyBox, -skyBox, -skyBox);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(skyBox, skyBox, -skyBox);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(skyBox, skyBox, skyBox);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(skyBox, -skyBox, skyBox);
	glEnd();
	// Left Face
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-skyBox, -skyBox, -skyBox);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-skyBox, -skyBox, skyBox);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-skyBox, skyBox, skyBox);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-skyBox, skyBox, -skyBox);
	glEnd();

}
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0, 0, 10, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	GLUquadric *quad;
	quad = gluNewQuadric();

	glRotatef(zRotated, 0.0f, 1, 0);
	oda();
	lightSourceAndSun();
	planetArrayOlustur();
	glFlush();
	glutSwapBuffers();
}

void resizeWindow(int x, int y)
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

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;
	case 32:
		for (int i = 0; i < 3; i++){
			planetsArray[i].planetSpin = -planetsArray[i].planetSpin;
		}
		glutPostRedisplay();		break;
	default:
		break;
	}
	switch (key) {

	default:		                                    break;
	}
}

char* filename1 = "./earth.bmp";
char* filename2 = "./sun.bmp";
char* filename3 = "./images/Galaxy/Galaxy_FT.bmp";
char* filename4 = "./images/Galaxy/Galaxy_BK.bmp";
char* filename5 = "./images/Galaxy/Galaxy_UP.bmp";
char* filename6 = "./images/Galaxy/Galaxy_DN.bmp";
char* filename7 = "./images/Galaxy/Galaxy_RT.bmp";
char* filename8 = "./images/Galaxy/Galaxy_LT.bmp";
char* filename9 = "./merkur.bmp";
char* filename10 = "./mars.bmp";


void idleFunc(void)
{

	yRotated -= 0.03;
	zRotated += 0.01;
	drawScene();
}
void gezegenHizlandir(){
	for (int i = 0; i < 3; i++){
		if (planetsArray[i].planetSpin < 0)
			planetsArray[i].planetSpin -= planetsArray[i].geciciplanetSpin / 2.0;
		if (planetsArray[i].planetSpin >= 0)
			planetsArray[i].planetSpin += planetsArray[i].geciciplanetSpin / 2.0;
	}
}
void gezegenYavaslat(){
	for (int i = 0; i < 3; i++){
		if (planetsArray[i].planetSpin<0.01&&planetsArray[i].planetSpin>-0.01){
			planetsArray[i].planetSpin = 0;
		}
		if (planetsArray[i].planetSpin < 0)
			planetsArray[i].planetSpin += planetsArray[i].geciciplanetSpin / 2.0;
		if (planetsArray[i].planetSpin > 0)
			planetsArray[i].planetSpin -= planetsArray[i].geciciplanetSpin / 2.0;
	}
}
void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN){
			gezegenHizlandir();
			glutPostRedisplay();
		}		break;
	case GLUT_MIDDLE_BUTTON:
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN){
			gezegenYavaslat();
			glutPostRedisplay();
		}break;
	default:

		break;
	}
}
void SpecialFunc(int key, int x, int y){
	switch (key)
	{
	case 100:
		cameraX -= 1;
		cameraZ -= 1;
		break;
	case 101:
		glutPostRedisplay();
		break;
	case 102:
		cameraX += 1;
		cameraZ -= 1;
		glutPostRedisplay();
		break;
	case 103:
		glutPostRedisplay();
		break;
	}
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	loadTextureFromFile(filename1, 1);
	loadTextureFromFile(filename2, 2);
	loadTextureFromFile(filename3, 3);
	loadTextureFromFile(filename4, 4);
	loadTextureFromFile(filename5, 5);
	loadTextureFromFile(filename6, 6);
	loadTextureFromFile(filename7, 7);
	loadTextureFromFile(filename8, 8);
	loadTextureFromFile(filename9, 9);
	loadTextureFromFile(filename10, 10);
	glutDisplayFunc(drawScene);
	glutMouseFunc(mouse);
	glutSpecialFunc(SpecialFunc);
	glutReshapeFunc(resizeWindow);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idleFunc);
	glutMainLoop();
	return 0;
}
