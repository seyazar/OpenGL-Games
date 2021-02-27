/*********
CTIS164 - Homework #4
----------
STUDENT : Seyma Yazar
SECTION :01
HOMEWORK: #4
----------
PROBLEMS:
----------
ADDITIONAL FEATURES:
*********/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "vec.h"
#include <time.h>
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
#define PI 3.1415
#define TIMER_PERIOD  15 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer
#define  triangleAmount 20
#define D2R 0.0174532

typedef struct {
	double x, y;
 }point_t;
typedef struct {
	float r, g, b;
}color_t;

typedef struct {
	vec_t   position;
	color_t color;
	vec_t   vel;
	bool activeredlight = true;  //for activating and deactivating lights
	bool activegreenlight = true;
	bool activebluelight = true;
} lights_t;

typedef struct {
	vec_t position;
	vec_t n;
} vertex_t;
typedef struct {
	
	bool showtargets = true;//target will be appear when this is active
	point_t center;
	float radius;
	float speed;
	float angle;
	int direct;//direction of the planets
	
} planet_t;

typedef struct {
	lights_t suncol;
	bool sunactive = true;//whether sun is active or not
} sun_t;

/* Global Variables for Template File */
sun_t sun;
lights_t light[3];
planet_t planet[3];
bool up = false, down = false, right = false, left = false;
bool activeTimer = false; 
int  winWidth, winHeight; // current Window width and height

color_t mulColor(float k, color_t c) {
	color_t tmp = { k * c.r, k * c.g, k * c.b };
	return tmp;
}

color_t addColor(color_t c1, color_t c2) {
	color_t tmp = { c1.r + c2.r, c1.g + c2.g, c1.b + c2.b };
	return tmp;
}


double distanceImpact(double d) {
	return (-1.0 / 350.0) * d + 1.0;
}


color_t calculateColor(lights_t source, vertex_t v) {
	vec_t L = subV(source.position, v.position);
	vec_t uL = unitV(L);
	vec_t N;
	float factor = dotP(uL, v.n) * distanceImpact(magV(L));
	return mulColor(factor, source.color);
}



void circle(int x, int y, int r)
{

	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void print(int x, int y, char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i<len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

void vprint(int x, int y, void *font, char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i<len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

void vprint2(int x, int y, float size, char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i<len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}
void initializelight() { //light initalizations random position etc.
	for (int i = 0; i < 3; i++) {
		light[i].position.x = rand()%300;
		light[i].position.y = rand()%300;
	}
	
	light[0].vel = { 3,  2 };
	light[1].vel = { -2, -1 };
	light[2].vel = { 3, -2 };
}
void displaylight() {//light drawings
	if (!light->activeredlight)//checks whether light is on or off
	{
		light[0].color = { 0.5,0.5,0.5 };
	}
	else
		light[0].color = { 1,0,0};

	if (!light->activegreenlight)
	{
		light[1].color = { 0.5,0.5,0.5 };
	}
	else 
		light[1].color = { 0,1,0 };

	if (!light->activebluelight)
	{
		light[2].color = { 0.5,0.5,0.5 };
	}
	else
		light[2].color = { 0,0,1 };

		for (int i = 0; i < 3; i++) {//light drawings
		glColor3f(light[i].color.r, light[i].color.g, light[i].color.b);
		circle(light[i].position.x, light[i].position.y, 10);
	}
		


}
void movefunc() { //function for moving the targets

	for (int i = 0; i < 3; i++) {
		if(planet[i].direct==1)
		planet[i].angle += planet[i].speed;// add random speed
		else
			planet[i].angle += -(planet[i].speed); //adding the other direction
		
		
		while (planet[i].angle > 360)
			planet[i].angle -= 360;
		planet[i].center = { sin(planet[i].angle * D2R)*planet[i].radius,cos(planet[i].angle * D2R)*planet[i].radius };//for circular movement.

	}


}

void initplanet() {//planet values initialization
	for (int i = 0; i < 3; i++) {
		planet[i].angle = rand() % 360;
		planet[i].speed = (rand() % 5)+1;
		planet[i].radius = (i + 1) * 80;
		planet[i].direct = rand() % 2;
	}
	
	movefunc();
}

void drawPlanet() {//planet drawings
	
	float angle;
	for (int j = 0; j < 3; j++) {
		glBegin(GL_TRIANGLE_FAN);
		glColor3f(0, 0, 0);

		glVertex2f(planet[j].center.x*cos(planet[j].angle*D2R), planet[j].center.y*sin(planet[j].angle*D2R));

		for (int i = 0; i <= 120; i++)
		{
			angle = 2 * i*PI / 120;
			float px = planet[j].center.x*cos(planet[j].angle*D2R) + 15 * cos(angle);
			float py = planet[j].center.y*sin(planet[j].angle*D2R) + 15 * sin(angle);
			vertex_t P = { {px,py}, {0,0.5} };
			P.n = unitV(subV({ px,py }, { planet[j].center.x*cos(planet[j].angle*D2R),planet[j].center.y*sin(planet[j].angle*D2R) }));
			color_t res = { 0,0,0 };
			for (int k = 0; k < 3; k++)
			{
				res = addColor(res, calculateColor(light[k], P));//calculating the lights

			}
			if(sun.sunactive)
			res = addColor(res, calculateColor(sun.suncol, P));//calculating sun
			glColor3f(res.r, res.g, res.b);
			glVertex2f(px, py);
		}
		glEnd();
	}
		
		
	}

void drawsun() {//Sun drawing
	if(sun.sunactive){
		sun.suncol.color = { 1,1,1 };
	}
	else
		sun.suncol.color = { 0.5,0.5,0.5 };
	glColor3f(sun.suncol.color.r, sun.suncol.color.g, sun.suncol.color.b);
	circle(0, 0, 20);
}
void backG() {

	glColor3f(0, 1, 1);
	glRectf(-400, -250, 400, -300);
	glColor3f(0, 0, 1);
	if(light->activeredlight)
	vprint(-380,-270,GLUT_BITMAP_HELVETICA_12,"F1 RED: ON ");
	else
		vprint(-380, -270, GLUT_BITMAP_HELVETICA_12, "F1 RED: OFF ");

	if (light->activegreenlight)
	vprint(-290, -270, GLUT_BITMAP_HELVETICA_12, "F2 GREEN: ON");
	else
		vprint(-290, -270, GLUT_BITMAP_HELVETICA_12, "F2 GREEN: OFF");
	if(light->activebluelight)
	vprint(-170, -270, GLUT_BITMAP_HELVETICA_12, "F3 BLUE: ON");
	else
		vprint(-170, -270, GLUT_BITMAP_HELVETICA_12, "F3 BLUE: OFF");
	if(sun.sunactive)
	vprint(-60, -270, GLUT_BITMAP_HELVETICA_12, "F4 SUN: ON");
	else
		vprint(-60, -270, GLUT_BITMAP_HELVETICA_12, "F4 SUN: OFF");
	if(activeTimer)
	vprint(50, -270, GLUT_BITMAP_HELVETICA_12, "F5 ANIMATION: ON");
	else
		vprint(50, -270, GLUT_BITMAP_HELVETICA_12, "F5 ANIMATION: OFF");

	vprint(200, -270, GLUT_BITMAP_HELVETICA_12, "F6: RESTART");

	glColor3f(0, 1, 1);
	vprint(-380, 260, GLUT_BITMAP_HELVETICA_18, "HOMEWORK #4");
	vprint(-380, 230, GLUT_BITMAP_HELVETICA_12, "by SEYMA YAZAR");
}
void display() {
	//
	// clear window to black
	//
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	displaylight();
	drawPlanet();
	backG();
	drawsun();

	glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

		

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;
	
	
	}
	if (key == GLUT_KEY_F1)
		light->activeredlight = !light->activeredlight;

	if (key == GLUT_KEY_F2)
		light->activegreenlight = !light->activegreenlight;

	if (key == GLUT_KEY_F3)
		light->activebluelight = !light->activebluelight;

	if (key == GLUT_KEY_F4)
		sun.sunactive = !sun.sunactive;
	if (key == GLUT_KEY_F5)
		activeTimer = !activeTimer;
		
	if (key == GLUT_KEY_F6)
	{
		initializelight();
		initplanet();
	}
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.
	if(activeTimer){
		

	for (int i = 0; i < 3; i++) {
		light[i].position = addV(light[i].position, light[i].vel);
		if (planet[i].direct == 1)
			planet[i].angle += planet[i].speed;// add random speed
		else
			planet[i].angle += -(planet[i].speed); //adding the other direction


		while (planet[i].angle > 360)
			planet[i].angle -= 360;
		
		if (light[i].position.x > 400|| light[i].position.x < -400) {// Reflection from Walls
			light[i].vel.x *= -1;
		}

		if (light[i].position.y > 300 || light[i].position.y < -250) {
			light[i].vel.y *= -1;
		}
	}
}

	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char *argv[]) {
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("Lighting by Seyma Yazar");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

	initializelight();
	initplanet();
#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}