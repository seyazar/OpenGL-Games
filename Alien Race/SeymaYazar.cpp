/*********
Allien Race by  Þeyma YAZAR
*********/
#include <ctime>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <random>
#include<time.h>
#include <datetimeapi.h>

#define WINDOW_WIDTH  680
#define WINDOW_HEIGHT 600
#define DURATION 1
#define TIMER_PERIOD 10// Period for the timer.
#define TIMER_ON  1 // 0:disable timer, 1:enable timer
#define D2R 0.0174532
#define PI 3.1415
#define SPDRANDOM 100 // speed range between 0-99
#define RANDOMREFRESH 100 //For non stop moving.

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
int a = 340;//half of winwindth
int x1 = 0, x2 = 0, x3 = 0, x4 = 0, x5 = 0;//different x variables for every alien 
int n = 300;//center of every alien
int winner = 0;
int dr1 = 1, dr2 = 1, dr3 = 1, dr4 = 1, dr5 = 1; //direction of the aliens
int millisec;
int sec=0;//second counter
int min = 0;//min counter
bool activeTimer =false;//It should be false at first because it should not move before space button.
int timercount = 0;
int spd1 = 0, spd2 = 0, spd3 = 0, spd4=0, spd5 = 0;//speed values

void circle (int x, int y, int r,int n)
{

	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + n*sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r,int n)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + n*sin(angle));
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
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
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

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
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
void spaceShips() {float angle;
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 1 * PI*i / 100;//for semicircle 
		glVertex2f(255 + 15 * cos(angle), 280 + 15 * sin(angle));
	}
	glEnd();
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 1 * PI*i / 100;
		glVertex2f(-255 + 15 * cos(angle), 280 + 15 * sin(angle));
	}
	glEnd();
	glColor3f(0, 1, 1);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 1 * PI*i / 100;
		glVertex2f(255 + 30 * cos(-angle), 280 + 20 * sin(-angle));//for other half of a semicycle make an -angle.
	}
	glEnd();
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 1 * PI*i / 100;
		glVertex2f(-255 + 30 * cos(-angle), 280 + 20 * sin(-angle));
	}
	glEnd();
	glColor4f(1, 1, 1, 0.2);
	glBegin(GL_TRIANGLES);
	glVertex2f(-255, 260);
	glVertex2f(-275, 210);
	glVertex2f(-235, 210);
	glVertex2f(255, 260);
	glVertex2f(275, 210);
	glVertex2f(235, 210);
	glEnd();
	glColor3f(1, 5,0);
	circle(255, 273, 3, 3);//little circle drawings.
	circle(240, 273, 3, 3);
	circle(270, 273, 3, 3);
	circle(-255, 273, 3, 3);
	circle(-240, 273, 3, 3);
	circle(-270, 273, 3, 3);
	
	
	
}

void backGround()  {
	//Lines between the rectangles
	
	glLineWidth(2.5);
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex2f(-a,200);
	glVertex2f(a,200);
	glVertex2f(-a, 100);
	glVertex2f(a, 100);
	glVertex2f(-a, 0);
	glVertex2f(a, 0);
	glVertex2f(-a, -100);
	glVertex2f(a, -100);
	glVertex2f(-a, -200);
	glVertex2f(a, -200);
	glEnd();
	glColor3f(0.192f, 0.192f, 0.192f);
	glRectf(-a, a, a, 200); 
	glColor3f(1, 1, 1);

	
	glColor3f(0.5f, 0.5f, 0.5f);
	glRectf(-170,275,170,240);
	glColor3f(1, 1, 1);
	vprint(-155, 250, GLUT_BITMAP_TIMES_ROMAN_24, "ALIEN RACE by Seyma Yazar");
	glColor3f(1, 1, 1);
		vprint(x1-a, 110, GLUT_BITMAP_9_BY_15, "#1"); vprint(x2-a, 10, GLUT_BITMAP_9_BY_15, "#2"); vprint(x3-a, -90, GLUT_BITMAP_9_BY_15, "#3"); vprint(x4-a, -190, GLUT_BITMAP_9_BY_15, "#4"); vprint(x5-a, -290, GLUT_BITMAP_9_BY_15, "#5");
		//Spaceship drawings:
		spaceShips();
		
}
// displays the prees f1 or space messages.
void displayMessage() {
	if ((dr1 == -1|| dr2 == -1|| dr3 == -1|| dr4 == -1|| dr5 == -1)&& (x1 == 10 || x2 == 10 || x3 == 10 || x4 == 10 || x5 == 10)) {
		glColor3f(0, 1, 1);
		for(int i=0;i<30;i++)
		vprint(-75, 220, GLUT_BITMAP_HELVETICA_12, "PRESS <F1> FOR RESTARTING");
	}
	else {
	glColor3f(0, 1, 1);
	vprint(-155, 220, GLUT_BITMAP_HELVETICA_12, "PRESS <SPACE> FOR STARTING / STOPPING ALIENS");
	}
	glColor3f(1, 1, 0);
	vprint(-50, 205, GLUT_BITMAP_HELVETICA_10, "PRESS <ESC> FOR EXIT");
}
//allien drawings.
void allienNum1() {
	glColor3f(1, 0, 0);
	circle(x1 - n, 140, 30,30);
	
	glColor3f(1,1,1);
	circle(x1 - 320, 160, 15,15);
	circle(x1 - 280, 160, 15,15);
	glColor3f(0, 0, 1);
	circle_wire((x1 - 320), 160, 15,15);
	circle_wire((x1 - 280), 160, 15,15);
	glBegin(GL_LINES);
	glVertex2f((x1 - 308), 160);
	glVertex2f((x1 - 292), 160);
	glEnd();
	glColor3f(0, 0, 0);
	circle((x1 - 320), 160, 5, 5);
	circle((x1 - 280), 160, 5, 5);
	glBegin(GL_TRIANGLES);
	glVertex2f((x1-320),140 );
	glVertex2f((x1-280),140);
	glVertex2f((x1-300), 120);
	glEnd();
	glColor3f(1, 1, 1);
	glBegin(GL_TRIANGLES);
	glVertex2f((x1-320), 140);
	glVertex2f((x1-310), 140);
	glVertex2f((x1-315), 135);

	glVertex2f((x1-310), 140);
	glVertex2f((x1-300), 140);
	glVertex2f((x1-305), 135);

	glVertex2f((x1-300), 140);
	glVertex2f((x1-290), 140);
	glVertex2f((x1-295), 135);

	glVertex2f((x1-290), 140);
	glVertex2f((x1-280), 140);
	glVertex2f((x1-285), 135);
	glEnd();
	
}
void allienNum2() {
	glColor3f(2.0f, 0.5f, 1.0f); 
	circle((x2 - n), 40, 30,30);
	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
	glVertex2f((x2-280),62);
	glVertex2f((x2 - 280), 75);
	glVertex2f((x2 - 300), 67);
	glVertex2f((x2 - 300), 79);
	glVertex2f((x2 - 320), 62);
	glVertex2f((x2 - 320), 72);
		glEnd();
		int r = 9;
		glColor3f(1, 1, 1); 
		circle(x2 - 280, 80, 9,9);
		circle(x2 - 300, 80, 9,9);
		circle(x2 - 320, 80, 9,9);
		glColor3f(0, 0, 0);
		glPointSize(6);
		glBegin(GL_POINTS);
		glVertex2f((x2-280),80);
		glVertex2f(x2 - 300, 80);
		glVertex2f(x2 - 320 , 80);
		glEnd();
		glRectf((x2-310),30,(x2-290),20);
		glColor3f(1, 1, 1);
		glBegin(GL_TRIANGLES);
		glVertex2f((x2-305),30);
		glVertex2f((x2 - 295), 30);
		glVertex2f((x2 - 300), 25);
		glEnd();
}
void allienNum3() {
		glColor3f(1,1,0);
	glRectf(x3 - 305, -25, x3 - 295,-65 );
	glColor3f(1.0f, 0.5f, 0.0f);
	circle(x3 - n, -25, 15,15);

	glColor3f(1.0f, 0.5f, 0.0f); 
	circle(x3 - n, -75, 20,20);
	
	glColor3f(1, 1, 1); 
	circle(x3 - 300, -25, 8,8);

	glColor3f(0, 0, 0);	
	circle(x3 - n, -25, 3,3);
	
	glColor3f(1, 0, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(x3-310,-50);
	glVertex2f(x3 - 310, -60);
	glVertex2f(x3 - 300, -55);
	glVertex2f(x3 - 290, -50);
	glVertex2f(x3 - 290, -60);
	glVertex2f(x3 - 300, -55); //tepe
	glEnd();
	glColor3f(0, 0, 0); 
	circle(x3 - n, -55, 3,3);
	

}
void allienNum4() {
	glColor3f(0, 1, 0);
	circle(x4 - n, -170, 25, 25);
	circle(x4 - n, -130, 20, 20);
	
	glColor3f(1, 1, 1);
	circle(x4 - n, -150, 8, 8);
	circle(x4 - n, -130, 10, 10);

	glColor3f(0, 0, 0);
	circle(x4 - n, -150, 4, 4);
	circle(x4 - n, -130, 5, 5);
	
	glColor3f(1, 0, 0); 
	glBegin(GL_LINES);
	glVertex2f(x4-310,-170);
	glVertex2f(x4 - 290, -170);
	glEnd();

}
void allienNum5() {
	glColor3f(0, 1, 1);
	glBegin(GL_LINES);
		glVertex2f(x5-290,-270);
		glVertex2f(x5 - 290, -285);
		glVertex2f(x5 - 310, -270);
		glVertex2f(x5 - 310, -285);
		glVertex2f(x5 - 285, -230);
		glVertex2f(x5 - 285, -220);
		glVertex2f(x5 - 315, -230);
		glVertex2f(x5 - 315, -220);
		glEnd();
	glColor3f(1, 1, 0); 
	circle(x5 - n, -250, 25,25);

	glColor3f(1, 1, 1);	
	circle(x5 - 285, -213, 8,8);
	circle(x5 - 315, -213, 8,8);
	
	glColor3f(0, 0, 0); 
	circle(x5 - 285, -213, 4, 7);
	circle(x5 - 315, -213,4, 7);
	glColor3f(0, 0, 0);
	circle(x5 - 300, -253, 10, 15);
	
	glColor3f(1, 0, 0);
	circle(x5 - 300, -264, 8, 4);
	glRectf(x5 - 292, -285,x5-280,-290);
	glRectf(x5 - 307, -285, x5 - 319, -290);


}
void printWinner() {

glColor3f(1, 1, 1); 
	vprint(230, 230, GLUT_BITMAP_HELVETICA_12, "WINNER");
	vprint(245, 215, GLUT_BITMAP_8_BY_13, "#%d", winner);
}
//calculating winner
void winnerDisplay() {

	if (!activeTimer) {
		printWinner();// if active timer is false at first it will show 0 as winner because every x is equal and they are not moving.
		return;
	}
		winner = 1;// winner is 1 because at first #1 is the max x.
		int maxx = x1;// making the max x x1 if any of these x's is greater than x1 winner will change depending on x's number
	if (dr1 == 1 && dr2 == 1 && dr3 == 1 && dr4 == 1 && dr5 == 1) {
		if (x2 > maxx)
		{
			maxx = x2;
			winner = 2;
		}
		if (x3 > maxx) {
			maxx = x3;
			winner = 3;
		}
		if (x4 > maxx) {
			maxx = x4;
			winner = 4;
		}
		if (x5 > maxx) {
			maxx = x5;
			winner = 5;
		}
		printWinner();
		return;

	}
	if (dr1 == -1 || dr2 == -1 || dr3 == -1 || dr4 == -1 || dr5 == -1){
		if (x2 < maxx)
		{
			maxx = x2;
			winner = 2;
		}
		if (x3 < maxx) {
			maxx = x3;
			winner = 3;
		}
		if (x4 < maxx) {
			maxx = x4;
			winner = 4;
		}
		if (x5 < maxx) {
			maxx = x5;
			winner = 5;
		}
		printWinner();
		return;
	}
	if (dr1 == -1 && x1 == 10 || x2 == 10 || x3 == 10 || x4 == 10 || x5 == 10) {
		for(int i=0;i<300;i++)
		printWinner();
	}
	
}
//displays the timer
void timerDisplay()
{
	
	
glColor3f(1, 1, 1);
	vprint(-275, 230, GLUT_BITMAP_8_BY_13, "TIMER");
	vprint(-285, 215, GLUT_BITMAP_8_BY_13, "%02d:%02d:%02d",min,sec,millisec);
	
	
}
//
// To display onto window using OpenGL commands
//
void display() {

	//
	// clear window to black
	//
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	backGround();
	
	allienNum1();
	allienNum2();
	allienNum3();
	allienNum4();
	allienNum5();
	winnerDisplay();
	displayMessage();
	timerDisplay();
	
	glutSwapBuffers();

}
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);
	if (key == ' ') //moving and stoping aliens with space button.
		activeTimer = !activeTimer;
	

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
	// if f1 is pressed restart the game
	if (key == GLUT_KEY_F1) {
		x1 = 0,x2 = 0 ,x3 = 0, x4 = 0, x5 = 0;
		dr1 = 1, dr2 = 1, dr3 = 1, dr4 = 1, dr5 = 1;
		activeTimer = false;
		winner = 0;
		sec = 0;
		min = 0;
		millisec = 0;
		timercount = 0;
		
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

	


	if (activeTimer == true) {
			
			if(timercount==0){
				
			spd1= rand() % SPDRANDOM+SPDRANDOM, spd2 = rand() % SPDRANDOM + SPDRANDOM, spd3 = rand() % SPDRANDOM + SPDRANDOM, spd4 = rand() % SPDRANDOM + SPDRANDOM, spd5 = rand() % SPDRANDOM + SPDRANDOM; //finding new random nums
			}
			x1 += (((spd1*(timercount+1))/RANDOMREFRESH) - ((spd1*timercount ) / RANDOMREFRESH))*dr1; //adding speed to aliens
			x2 += (((spd2*(timercount + 1)) / RANDOMREFRESH) - ((spd2*timercount) / RANDOMREFRESH))*dr2;
			x3 += (((spd3*(timercount + 1)) / RANDOMREFRESH) - ((spd3*timercount) / RANDOMREFRESH))*dr3;
			x4 += (((spd4*(timercount + 1)) / RANDOMREFRESH) - ((spd4*timercount) / RANDOMREFRESH))*dr4;
			x5 += (((spd5*(timercount + 1)) / RANDOMREFRESH) - ((spd5*timercount) / RANDOMREFRESH))*dr5;
			timercount=(timercount+1)%RANDOMREFRESH;
			if (x1 >= 600) //changing the direction of aliens 
			{
				x1 = 600-(x1%600);
				dr1 = -1;
			}
			if (dr1 == -1 && x1<= 10){
				x1 = 10;
			activeTimer=false; //when any alien reachs the finish line active timer is false and race finishes.
			}
			if (x2 >= 600)
			{
				x2 = 600 - (x2 % 600);
				dr2 = -1;
			}
			if (dr2 == -1 && x2 <= 10) {
				x2 = 10;
				activeTimer=false;
			}if (x3 >= 600)
			{
				x3 = 600 - (x3 % 600);;
				dr3 = -1;
			}
			if (dr3 == -1 && x3 <= 10) {
				x3 = 10;
				activeTimer=false;
			}
			if (x4 >= 600)
			{
				x4 = 600 - (x4 % 600);;
				dr4 = -1;
			}
			if (dr4 == -1 && x4 <= 10) {
				x4 = 10;
				activeTimer=false;
			}
			if (x5 >= 600)
			{
				x5 = 600 - (x5 % 600);;
				dr5 = -1;
			}
			if (dr5 == -1 && x5 <= 10) {
				x5 = 10;
				activeTimer=false;
			}
}
			

	
		// to refresh the window it calls display() function
		glutPostRedisplay(); // display()
}
#endif
void calculateTime(int )//calculating time as min,sec,millisec
{
	glutPostRedisplay();
	glutTimerFunc(1000.0 / 60.0, calculateTime, 0);
	if (activeTimer == true)
	{
		millisec++;
		if (millisec == 60)
		{
			sec++;
			millisec = 0;
		}
		if (sec == 60)
		{
			min++;
			sec = 0;
		}
	}

}
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
	glutCreateWindow("SEYMA YAZAR - ALIEN RACE");
	srand(time(NULL));


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

#if  TIMER_ON == 1,
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	glutTimerFunc(1000.0 / 60.0, calculateTime, 0);

	glutPostRedisplay();
	
#endif


	Init();

	glutMainLoop();
}
