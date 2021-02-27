/*********
CTIS164 - HomeWork-3
----------
STUDENT :Seyma Yazar
SECTION :01
HOMEWORK:#3
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
#include <time.h>

#define WINDOW_WIDTH  900
#define WINDOW_HEIGHT 750

#define  TIMER_PERIOD  16 
#define  TIMER_ON     1     // 0:disable timer, 1:enable timer

#define D2R 0.0174532
#define MAX_FIRE 20  
#define FIRE_RATE 8 

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false, spacebar = false, rightclick = false;
int  winWidth, winHeight; // current Window width and height
bool activetimer = false; 




typedef struct {
	double x, y;
} point_t;

typedef struct {
	point_t pos;    // position of the object
	float   angle;  // view angle 
} player_t;

typedef struct {
	point_t pos;
	float angle;
	bool active;
} fire_t;
typedef struct {
	int r, g, b;
	bool showtargets=true;//target will be appear when this is active
	point_t center;
	float radius;
	float speed;
	float angle;
	int clockwise;//direction of the targets
} target_t;



player_t p = { { 0,0 }, 45};
fire_t   fr[MAX_FIRE];
int fire_rate = 0;
target_t target[3];

void circle(int x, int y, int r)
{
#define PI 3.1415
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


// display text with variables.
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
void movefunc() { //function for moving the targets
	
	for (int i = 0; i < 3; i++) {
		
		target[i].angle += target[i].speed;// add random speed
	
		if (target[i].angle > 360)
			target[i].angle -= 360;
		target[i].center = { sin(target[i].angle * D2R)*target[i].radius,cos(target[i].angle * D2R)*target[i].radius };//for circular movement.
		
	}


}

void drawBackground() {
	glColor3f(1, 1, 1);
	vprint(-400, 330, GLUT_BITMAP_HELVETICA_12, "HOMEWORK #3");
	vprint(-400, 310, GLUT_BITMAP_HELVETICA_12, "by SEYMA YAZAR");
	glBegin(GL_LINES);// x and y 
	glVertex2f(0, -400);
	glVertex2f(0, 400);
	glVertex2f(-450, 0);
	glVertex2f(450, 0);
	glEnd();
	if (!activetimer&&target[0].showtargets == false && target[1].showtargets == false && target[2].showtargets == false) {
		glColor3f(1, 0, 1);
		vprint(20, 0, GLUT_BITMAP_HELVETICA_18, "CLICK FOR RESTART"); //message when game ends.
	
	}
}
void drawPlayer(player_t tp) {
	glColor3f(1, 1, 0);
	glLineWidth(3);
	glBegin(GL_TRIANGLES);
	glVertex2f(tp.pos.x, tp.pos.y);
	glVertex2f(tp.pos.x + 20, tp.pos.y + 20);
	glVertex2f(tp.pos.x + 50 * cos(tp.angle * D2R), tp.pos.y + 50 * sin(tp.angle * D2R));
	glEnd();
	

	glColor3f(1, 1, 1);
	vprint(tp.pos.x-11 * cos(tp.angle * D2R)*36, tp.pos.y - 11 * sin(tp.angle * D2R)*36, GLUT_BITMAP_8_BY_13, "%.0f", tp.angle);
}

void drawFires() {
	for (int i = 0; i < MAX_FIRE; i++) {
		if (fr[i].active) {
			glColor3f(1,0 ,0);
			circle(fr[i].pos.x, fr[i].pos.y, 6);
		}
	}
}
void drawwires() {
	
	//circle wires
		glColor3f(1, 1, 1);
		circle_wire(0,0,120);
		circle_wire(0, 0, 240);
		circle_wire(0, 0, 360);
		

}
void initplanet() {//target values initialization
	for (int i = 0; i < 3; i++) {
		target[i].angle = rand() % 360;
		target[i].speed = rand() % 10;
		target[i].radius = (i+1)*120;
		target[i].showtargets = true;
		target[i].r = rand() % 250;
		target[i].g = rand() % 250;
		target[i].b = rand() % 250;
		

	}
	movefunc();
}

void drawPlanet() {//target drawings
	
	for (int i=0; i<3; i++) {
       if(target[i].showtargets){
		glColor3ub(target[i].r, target[i].g, target[i].b);//random colors
		circle(target[i].center.x , target[i].center.y, 25);
		glColor3f(1, 0, 1);
		vprint(target[i].center.x, target[i].center.y, GLUT_BITMAP_8_BY_13, "%0.f", target[i].angle);

	}
}
}

void display()
{

	//
	// clear window to black
	//
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	drawBackground();
	drawFires();
	drawPlayer(p);
	drawPlanet();
	drawwires();
	glutSwapBuffers();

}


int findAvailableFire() {
	for (int i = 0; i < MAX_FIRE; i++) {
		if (fr[i].active == false) 
			return i;
	}
	return -1;
}

void turnPlayer(player_t *tp, float inc) {
	tp->angle += inc;
	if (tp->angle > 360) tp->angle -= 360;
	if (tp->angle < 0) tp->angle += 360;


}
//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);
	if (key == ' ')
		spacebar = true;
	// to refresh the window it calls display() function
	//glutPostRedisplay() ;
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);
	if (key == ' ')
		spacebar = false;
	// to refresh the window it calls display() function
	//glutPostRedisplay() ;
}


void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;

	}
	// to refresh the window it calls display() function
	//glutPostRedisplay() ;
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
	//glutPostRedisplay() ;
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
	// fires when right mouse click
	if ((button == GLUT_LEFT_BUTTON) && (stat == GLUT_DOWN))
	{
		rightclick = true;
		
	}

	if ((button == GLUT_LEFT_BUTTON) && (stat == GLUT_UP))
	{
		rightclick = false;
	}

	// to refresh the window it calls display() function
	//glutPostRedisplay() ; 
}


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
	//glutPostRedisplay() ;
}

void mousemove(int xm, int ym) {
	//mouse movement angle
	p.angle = atan2f(ym, xm)*(180 / PI);
	if (p.angle < 0)
		p.angle += 360;
}
void onMove(int x, int y) {
	// Write your codes here.
	int xm, ym;
		xm = x - winWidth / 2;
	 ym = winHeight / 2 - y;

	mousemove(xm, ym); //player moves as mouse moves
	// to refresh the window it calls display() function
	glutPostRedisplay() ;
}

bool testCollision(fire_t fr, target_t t) {
	float hitx = t.center.x - fr.pos.x;
	float hity = t.center.y - fr.pos.y;
	float hit = sqrt(hitx*hitx + hity*hity);
	return hit <= t.radius;
}


#if TIMER_ON == 1
void onTimer(int v) {
	if (rightclick&&!activetimer)//starts game with a click
	activetimer = true;
	if (activetimer) {
		movefunc();
	}
	if (activetimer&&target[0].showtargets == false && target[1].showtargets == false && target[2].showtargets == false) {

		activetimer = false;
	}//stoping the game after every object is destroyed.

	if (rightclick && !activetimer&&target[0].showtargets == false && target[1].showtargets == false && target[2].showtargets==false) {
		target[0].showtargets = true;
		target[1].showtargets = true;
		target[2].showtargets = true;
		activetimer = true;
	}//restarting the targets

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.

	

	if (rightclick&& fire_rate == 0) {
		int availFire = findAvailableFire();
		if (availFire != -1) {
			fr[availFire].pos = p.pos;
			fr[availFire].angle = p.angle;
			fr[availFire].active = true;
			fire_rate = FIRE_RATE;
		}
	}

	if (fire_rate > 0) fire_rate--;

	// Move all fires that are active.
	for (int i = 0; i< MAX_FIRE; i++) {
		if (fr[i].active) {
			fr[i].pos.x += 10 * cos(fr[i].angle * D2R);
			fr[i].pos.y += 10 * sin(fr[i].angle * D2R);

			if (fr[i].pos.x > 370 || fr[i].pos.x < -370 || fr[i].pos.y>370 || fr[i].pos.y < -370) {
				fr[i].active = false; //fire range
			}
			

			if (testCollision(fr[i], target[i])) {
				fr[i].active = false;
				target[i].showtargets = false;//remove the target if it is hit 
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


void main(int argc, char *argv[])
{
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Seyma_Yazar_HW#3");
	initplanet();
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

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}