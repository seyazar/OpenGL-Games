/*********
CTIS164 - Template Source Program
----------
STUDENT :Seyma Yazar
SECTION :01
STU_ID:21703784
HOMEWORK:Hit the Target 
----------
PROBLEMS: 
There are some problems with fire it is not working right because of that fire cannot hit the target.
fire 
when space is active fire is just changing its position for one time.
The score calculations is written but since the firefunc is not working the calculations also not working.
*********/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include<time.h>
#include <random>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD  1000 // Period for the timer.
#define TIMER_ON        1 // 0:disable timer, 1:enable timer
#define SPEED 40
#define D2R 0.0174532
typedef struct {
	int x = 0, y = 0;
}position_t;//structure of the positions 
typedef struct {
	GLubyte red, gre, blu;
	position_t pos;
	
}fish_t; //fish values(color,position).
typedef struct {
	position_t pos;
	int speed=20;
	bool fireactive = false;
}fire_t;//fire values
fish_t fish;
fish_t *fisharr;//array for the fish infos
bool up = false, down = false, right = false, left = false;
bool space = false;
int  winWidth, winHeight; // current Window width and height

bool activetimer =false;
int sec=20, milsec=0;
int xboat=60, yboat=155;//first position of the boat
int xb = 0;//movement of the boat
int fishnum;//total fish value

int lastpoint = 0, totalpoint = 0;
int fishcount = 0, huntedfish = 0; //score table values

void circle(int x, int y, int r,int n)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + n*sin(angle));
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

void seaweed(int xsea,int ysea,int lenght) {
	glBegin(GL_LINES);
	glVertex2f( xsea, ysea);
	glVertex2f( xsea, ysea + 40+lenght);
	glVertex2f(xsea, ysea);
	glVertex2f(xsea+20, ysea + 40+lenght);
	glVertex2f(xsea, ysea);
	glVertex2f(xsea-20, ysea+40+lenght);
	glEnd();
}//seawead drawings.
void backGround() {
	glColor3f(0, 0.69, 0.87);
	glRectf(-WINDOW_WIDTH / 2 - 130, 150, WINDOW_WIDTH / 2, 300);
	//sand
	glColor3ub(74, 70, 0);
	glRectf(-WINDOW_WIDTH/2,-WINDOW_HEIGHT/2, WINDOW_WIDTH / 2, -WINDOW_HEIGHT / 2+30);
	//cloud drawings
	glColor3f(1, 1, 1);
	circle(-300, 250, 20,20);
	circle(-270, 250, 20, 20);
	circle(-240, 250, 20, 20);
	circle(-270, 260, 30, 20);

	circle(230, 250, 20, 20);
	circle(200, 250, 20, 20);
	circle(170, 250, 20, 20);
	circle(200, 260, 30, 20);
	//underwater 
	glColor3f(0, 1, 0);
	seaweed(-300,-270,0);
	seaweed(200, -270,0);
	seaweed(50, -270, 0);
	seaweed(-50, -270, 0);
	glColor3f(1, 1, 0);
	seaweed(-340, -270,0);
	seaweed(240, -270,0);
	glColor3f(1, 0, 0);
	seaweed(-375, -270, 23); 
	seaweed(0, -270, 25);

	//f1 message
	if (sec == 0 && milsec == 0 && !activetimer)
	{
		glColor3f(1, 0, 1);
		vprint(-180, -285, GLUT_BITMAP_HELVETICA_12, "< PRESS F1 FOR PLAYING AGAIN >");
	}
}
void scoreTable() {
	glColor3f(0, 0, 0);
	glRectf(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, WINDOW_HEIGHT / 2-30, -300);
	glColor3f(1, 1, 0);
	vprint(285, 240,GLUT_BITMAP_HELVETICA_12, "REMAINING TIME");//timer display
	vprint(315, 220,GLUT_BITMAP_HELVETICA_18 , "%d:%d", sec, milsec);
	glColor3f(1, 0, 0);
	vprint(295, 160, GLUT_BITMAP_HELVETICA_12, "FISH COUNT");
	vprint(325, 140, GLUT_BITMAP_HELVETICA_18, "%d", fishcount);
	glColor3f(0, 1, 0);
	vprint(295, 80, GLUT_BITMAP_HELVETICA_12, "HUNTED FISH");
	vprint(325, 60, GLUT_BITMAP_HELVETICA_18, "%d", huntedfish);
	glColor3f(0, 0, 1);
	vprint(295, 0, GLUT_BITMAP_HELVETICA_12, "LAST SCORE");
	vprint(325, -30, GLUT_BITMAP_HELVETICA_18, "%d", lastpoint);
	glColor3f(1, 0, 1);
	vprint(295, -80, GLUT_BITMAP_HELVETICA_12, "TOTAL SCORE");
	vprint(325, -100, GLUT_BITMAP_HELVETICA_18, "%d", totalpoint);
} //score table drawing
void fisherman() {//boat drawing
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex2f(WINDOW_WIDTH / 2-130, 150);
	glVertex2f(-WINDOW_WIDTH / 2, 150);
	glEnd();
	glColor3ub(76, 50, 56);
		glRectf(xboat+xb,yboat,-xboat+xb,yboat+30);
	glBegin(GL_TRIANGLES);
	glVertex2f(xboat+xb, yboat);
	glVertex2f(xboat+xb, yboat+30);
	glVertex2f(xboat+30+xb, yboat+30);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex2f(-xboat+xb, yboat);
	glVertex2f(-xboat + xb, yboat+30);
	glVertex2f(-xboat-30 + xb, yboat+30);
	glEnd();
	glColor3f(0, 1, 1);
	glBegin(GL_TRIANGLES);
	glVertex2f(xboat - 60 + xb, yboat + 30);
	glVertex2f(xboat - 60 + xb, yboat + 90);
	glVertex2f(xboat  + xb, yboat + 30);
	
	glEnd();

	int size = 5;
	glLineWidth(size);

	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(xboat-60 + xb, yboat + 30);
	glVertex2f(xboat-60 + xb, yboat+90);
	glEnd();
}

void fishinit() { //fish values for every other fish
	fishnum = rand() % 20+10;
	fisharr = (fish_t*)malloc(fishnum * sizeof(fish_t)); //array tha cointains the fish information
	int i;
	
		for (i = 0; i < fishnum; i++) {//stores the fish info into an array
			fish_t fish ;
			fish.pos.y = rand() % 200 - 150;//random pos for the first display
			fish.red = rand() % 256, fish.gre = rand() % 256, fish.blu = rand() % 256;//random colors for every fish
			fish.pos.x = -rand() % 400;
			fisharr[i] = fish;

		
	}
}
void fishdisplay() {//fish drawing
	
	for (int i = 0; i < fishnum; i++) {
		fish_t fish = fisharr[i];
		glColor3ub(fish.red ,fish.gre, fish.blu);
		circle(fish.pos.x - WINDOW_WIDTH / 2, fish.pos.y, 20, 10);
		glBegin(GL_TRIANGLES);
		glVertex2f(fish.pos.x - (WINDOW_WIDTH / 2) - 10, fish.pos.y);
		glVertex2f(fish.pos.x - (WINDOW_WIDTH / 2) - 25, fish.pos.y + 15);
		glVertex2f(fish.pos.x - (WINDOW_WIDTH / 2) - 25, fish.pos.y - 15);
		glEnd();
	}
	
}

void movefire() {
	if (space == true)
	{
		fire_t *fire; //drawing of the arrow.
		glColor3f(0, 0, 0);
		glLineWidth(4);
		glBegin(GL_LINES);
		glVertex2f(fire->pos.x+xb,fire->pos.y);
		glVertex2f(fire->pos.x+xb, fire->pos.y+10);
		glEnd();
	}

}
void display() {
	glClearColor(0, 0, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	backGround();
	scoreTable();
	fishdisplay();
		fisherman();
		movefire();
		
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
	if (key == ' ') {
	
		space = true;  //fire when space button is active
	}
	
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
	if (key == GLUT_KEY_F1) { //starting & restarting the game 
		activetimer =true;

		fishinit();
		milsec = 0;
		sec = 20;
		
	}
	if (activetimer == false) 
	{ return;
	}
	if (key == GLUT_KEY_LEFT)
	{
		xb-=10;//movement of the boat
		
	}
	if (key == GLUT_KEY_RIGHT)
	{
		xb+=10;//movement of the boat
	
	}
	
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;
	}

	// to refresh the window it calls display() function
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


void onMove(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {
	fire_t fire;

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	fish_t fish;
	// Write your codes here.
	if (activetimer == true)
	
		for (int i = 0; i < fishnum; i++) {
			fisharr[i].pos.x += 50;//movement of the fish

				
		}
		if (space && !(fire.fireactive))
		
			fire.fireactive = true;
			if (fire.fireactive) 
			fire.pos.y += 12; //movement of the fire

		
			if (fire.pos.y == fish.pos.y&&fire.pos.x == fish.pos.x) {
				lastpoint = 5;//if fire is in the middle of the fishes gain 5pts.
			
				if(fire.pos.y <fish.pos.y+5&& fire.pos.y>fish.pos.y - 5 &&fire.pos.y != fish.pos.y|| fire.pos.x <fish.pos.x + 10 && fire.pos.x>fish.pos.x - 10 && fire.pos.x != fish.pos.x)
					lastpoint = 3;//if fire is  not  in the middle of the fishes gain 3pts.
		
			totalpoint += lastpoint;//total score.
		}
		
	

	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif
void calculateTime(int)//calculating time as sec,millisec
{
	
	glutTimerFunc(1000.0 / 60.0, calculateTime, 0);
	
	if (activetimer == true)
	{
		
		
		if (milsec == 0)
		{
			sec--;
			milsec = 60;
		}
		milsec--;
	}
	if (sec == 0 && milsec == 0) {
		activetimer = false;
	}
	glutPostRedisplay();
}
void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char *argv[]) {
	srand(time(NULL));//for making different random values.
	
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Seyma Yazar_fish hunt");

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
	glutTimerFunc(1000.0 / 60.0, calculateTime, 0);

	glutPostRedisplay();
#endif

	Init();

	glutMainLoop();
}