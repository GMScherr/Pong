#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
//Compilation flags g++ pong.c -lGL -lGLU -lglut;
//Defines screen size 
#define sizex 500
#define sizey 500
//Game state flag, the mother of all "magic numbers". Changes game behaviour depending on the flag. 0 = Game, 1 = Pause
int gamestate = 0;
//Ball position
int ballx = 0;
int bally = 0;
//Ball speed vector
int ballspeedx = 1;
int ballspeedy = 1;
//Ball speed modifier, makes the ball faster
float ballspeedmod = 10;
//Ball rotation angle
float ballrotateangle = 0;
//Top player cursor position
int bar1x = 0;
int bar1y = 500;
//Bottom player cursor position
int bar2x = 0;
int bar2y = -490;
//Global keyboard vector
int keyboard [127];


void setup(){
	glClearColor (0, 0, 0, 0);
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

	if (gamestate == 0){//Draws the ball when the gamestate = 0 (Gameplay)
		glPushMatrix();
			glTranslatef(ballx,bally,0);
			glRotatef(ballrotateangle,0,0,1);
			ballrotateangle = ballrotateangle + 20;
			if (ballrotateangle>360)
				ballrotateangle = ballrotateangle - 360;
			glBegin(GL_POLYGON);
				glVertex2f(-20,-20);
				glVertex2f(20,-20);
				glVertex2f(20,20);
				glVertex2f(-20,20);
			glEnd();
			glPopMatrix();
		}
	if (gamestate == 1){//Same as above but for gamestate = 1 (Paused). The difference is that the ball's rotation speed isn't updated here, meaning the ball maintains its angle when the game is paused
		glPushMatrix();
			glTranslatef(ballx,bally,0);
			glRotatef(ballrotateangle,0,0,1);
			glBegin(GL_POLYGON);
				glVertex2f(-20,-20);
				glVertex2f(20,-20);
				glVertex2f(20,20);
				glVertex2f(-20,20);
			glEnd();
			glPopMatrix();
	}
}

void resetball (){
	ballx = 0;
	bally = 0;
	ballspeedmod = 10;
}

void gameloop(int valor){

	if(keyboard[27]){
		switch(gamestate){
			case 0:
				gamestate = 1;//Changes gamestate to 1 (Paused state) when ESC is pressed
				break;
			case 1:
				gamestate = 0;//Continues game when the game is paused and the ESC key is pressed
				break;
		}
		keyboard[27] = 0;//Setting ESC to 0 here ensures that the ESC key toggles the pause menu
	}	

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
		//All logic below here is based off of them juicy magical numbers because it was made on prototype phase. Alas, I am too lazy to change the code so please don't change the window size.
		if (ballx>sizex-170)//Reflects the ball off the sides
			ballspeedx = -1;
		if (ballx<-sizex+170)
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
	drawball();
	glColor4f(1,1,1,0.1);
	glBegin(GL_POLYGON);
		glVertex2f(-200,-200);
		glVertex2f(200,-200);
		glVertex2f(200,200);
		glVertex2f(-200,200);
	glEnd();
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
   //Sets up the size of the screen. This makes the screen 300 pixels taller than it is wide.
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

