#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
//Compilation flags g++ pong.c -lGL -lGLU -lglut;
//Defines
#define sizex 500
#define sizey 500
//Game state flag. Prints different screens for different states. 0 = Game, 1 = Pause
int gamestate = 0;
//Ball position
int ballx = 0;
int bally = 0;
//Ball speed vector
int ballspeedx = 1;
int ballspeedy = 1;
//Ball speed modifier, makes the ball faster
float ballspeedmod = 10;
//Top player cursor position
int bar1x = 0;
int bar1y = 500;
//Bottom player cursor position
int bar2x = 0;
int bar2y = -490;
//Global keyboard vector
int keyboard [127];


void setup(){
	glClearColor (0, 0, 0, 1);
	for (int i = 0;i<127;i++)
		keyboard[i]=0;
}

void drawbar(int x,int y){
	glColor3f (1,1,1);
	glBegin(GL_POLYGON);
		glVertex2f(x-70,y);
		glVertex2f(x-70,y-10);
		glVertex2f(x+70,y-10);
		glVertex2f(x+70,y);
	glEnd();
}

void drawball(){
	glColor3f(1,0,0);
	glBegin(GL_POLYGON);
		glVertex2f(-20+ballx,-20+bally);
		glVertex2f(20+ballx,-20+bally);
		glVertex2f(20+ballx,20+bally);
		glVertex2f(-20+ballx,20+bally);
	glEnd();
}

void resetball (){
	ballx = 0;
	bally = 0;
	ballspeedmod = 10;
}

void gameloop(int valor){

	if ((gamestate==0)&&(keyboard[27])==1)//Changes gamestate to 1 (Paused state) when ESC is pressed
		gamestate = 1;
	if ((gamestate==1)&&(keyboard[122]))//Continues game when the game is paused and the z key is pressed
		gamestate = 0;
	if ((gamestate==1)&&(keyboard[120]))//Exits game when the game is paused and the x key is pressed
		exit(0);

	if (gamestate==0){
		if (keyboard[111]==1)//Moves the player's cursors
			bar1x = bar1x -10;
		if (keyboard[112]==1)
			bar1x = bar1x +10;
		if (keyboard[113]==1)
			bar2x = bar2x -10;
		if (keyboard[119]==1)
			bar2x = bar2x +10;
		
		ballx = ballx + ballspeedx*ballspeedmod;//Moves the ball in the x axis
		bally = bally + ballspeedy*ballspeedmod;//Moves the ball in the y axis

		if (ballx>330)//Reflects the ball off the sides
			ballspeedx = -1;
		if (ballx<-330)
			ballspeedx = 1;

		if(bar1x<-280)//Limits the movement of the player's cursors to stay within the screen
			bar1x=-280;
		if(bar1x>280)
			bar1x=280;
		if(bar2x<-280)
			bar2x=-280;
		if(bar2x>280)
			bar2x=280;

		if(ballspeedy==1){
			if((bally>=480)&&(bally<=500)&&((ballx>bar1x-90)&&(ballx<bar1x+90))){//Reflects the ball if it hits the top bar
				ballspeedy = -1;
				ballspeedmod = ballspeedmod*1.05;//Accelerates the ball whenever it reflects off the racket by 5%
			}
		}
		else{
			if((bally<=-480)&&(bally>=-500)&&((ballx>bar2x-90)&&(ballx<bar2x+90))){//Same as above but for the bottom bar
				ballspeedy = 1;
				ballspeedmod = ballspeedmod*1.05;
			}
		}
		if (bally>500||bally<-500)
			resetball();
	}
	
	glutPostRedisplay();
	glutTimerFunc(33,gameloop,0);
}

void escfunc (){
	glBegin(GL_POLYGON);
		glVertex2f(-200,-200);
		glVertex2f(200,-200);
		glVertex2f(200,200);
		glVertex2f(-200,200);
	glEnd();
	drawball();
	drawbar(bar1x,bar1y);
	drawbar(bar2x,bar2y);
}

void drawscene(){
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1,1,1);

	if(gamestate==1)
			escfunc();

	else{
		glClear(GL_COLOR_BUFFER_BIT);
		glFlush();
	}

	if(gamestate==0){
		drawball();
		drawbar(bar1x,bar1y);
		drawbar(bar2x,bar2y);
	}
	glFlush();
}


void keypress (unsigned char key, int x, int y){
	keyboard[key] = 1;
}

void keyrelease (unsigned char key, int x, int y){
	keyboard[key] = 0;
}

void redimensionada(int width, int height) {
   glViewport(0, 0, width, height);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-sizex+150, sizex-150, -sizey, sizey, -1, 1);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

int main(int argc, char** argv) {
   glutInit(&argc, argv);
   glutInitContextVersion(1, 1);
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
   //Stuff not worth looking at (yet)
   glutInitWindowSize(700, 1000);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Pong Prototype");
   //Self explanatory names
   glutDisplayFunc(drawscene);
   glutKeyboardFunc(keypress);
   glutKeyboardUpFunc(keyrelease);
   glutReshapeFunc(redimensionada);
   //No idea how reshaping works so I'll just copy the teacher's code
   setup();
   glutTimerFunc(33,gameloop,0);
   glutMainLoop();
   //The main loop proper, receives no parameters (I think)
   }