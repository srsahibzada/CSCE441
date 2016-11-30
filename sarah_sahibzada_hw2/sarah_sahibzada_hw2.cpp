/*
	SARAH SAHIBZADA
	CSCE 441 - COMPUTER GRAPHICS
	ASSIGNMENT 2
	UNCOMMENT LINE 289 TO VERIFY THAT SPEED IS CHANGING
*/

#include <GL/glut.h>
#include <cstdlib> // for null
#include <cstdio> //for printf
#include <iostream> // for debugging purposes
// GLOBALS, DEFINES, ET CETERA
//character constants for readability
#define C 99
#define M 109
#define Y 121
#define W 119
#define ONE 49
#define TWO 50
#define THREE 51

//initial values associated with speed
#define INITIAL_SPEED 0.5
#define INITIAL_ANGLE 0.0
#define MAX_ANGLE 360
#define MAX_NEG_ANGLE -360
#define MIN_SPEED 0.01
#define MAX_SPEED 359 // i don't anticipate needing this, as speed only increase/decreases w/in window bounds
#define ZERO 0.0
#define PUSH 0.05 // speed only increases/decreases in bounds, as such is more likely to hit
				  // the minimum than the maximum. this value 'pushes' it out of min condition
				  // once in order to allow the algorithm to proceed
//initial values associated with color
#define INITIAL_INTENSITY 0.5
#define MIN_INTENSITY 0.2
#define MAX_INTENSITY 1.0

// constants associated with the default height i've set
// this program accommodates changes and works with them, but i've tested
// it most heavily with these values
#define WIN_HEIGHT 600
#define WIN_WIDTH 600

#define DRAG_NORMALIZE 10 //this value is honestly a magic constant

//openGL constants and globals
static GLfloat spin = INITIAL_ANGLE; // start in neutral
static GLfloat speed = INITIAL_SPEED;  // rotate 0.5 degrees
//todo: shove this in a struct
GLboolean cyan = false;
GLboolean magenta = false;
GLboolean white = true;
GLboolean yellow = false;
GLboolean mouse_pressed = false;
GLboolean one = false; // point representation
GLboolean two = false; // lines
GLboolean three = true; //default solid polygon repesentation

//initialize intensities
GLfloat alpha = INITIAL_INTENSITY;
GLfloat red_intensity = INITIAL_INTENSITY;
GLfloat blue_intensity = INITIAL_INTENSITY;
GLfloat green_intensity = INITIAL_INTENSITY;
GLint width  = WIN_WIDTH;
GLint height = WIN_HEIGHT;
GLint gl_home_x = 0;
GLint gl_home_y = 0;

//to make mouse IO more readable
enum mouse_button{RAISED = -1, LEFT, CENTER, RIGHT};

int mouse = RAISED; //start here by default

//define the start points here
GLfloat ax = -25.0;
GLfloat ay = -25.0;
GLfloat bx = 25.0;
GLfloat by = 15.0;
GLfloat cx = 0.0;
GLfloat cy = 25.0;

//useful globals in cpp file
int home_x = width/2;
int home_y = height/2;
int start_x;
int start_y;
int last_x; 
int last_y;

using namespace std; // used with iostream for debugging


void init(void) {
	glClearColor(0.5,0.5,0.5,0.0);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(15.0);
	//gluOrtho2D(0.0,1200,0.0,600);
	glShadeModel(GL_FLAT);
}

void resetAlpha() {
	alpha = 0.5;
}	
//set global state variables associated with present color
void setColor(char c) {
	switch(c) {
	case C:
		cyan = true;
		magenta = false;
		white = false;
		yellow = false;
		break;
	case M:
		cyan = false;
		magenta = true;
		white = false;
		yellow = false;
		break;
	case Y:
		cyan = false;
		magenta = false;
		white = false;
		yellow = true;
		break;
	case W:
		cyan = false;
		magenta = false;
		white = true;
		yellow = false;
		break;
	default:
		break;
	}
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glRotatef(spin,0.0,0.0,1.0);
	 glColor3f(0.0,0.0,0.0);
	 if (one) {
		glBegin(GL_POINTS);
			glVertex2f(ax, ay);
			glVertex2f(bx, by);
			glVertex2f(cx, cy);
		glEnd();
	 }
	 if (two) {
		glBegin(GL_LINES);
			glVertex2f(ax, ay);
			glVertex2f(bx, by);
			glVertex2f(bx, by);
			glVertex2f(cx, cy);
			glVertex2f(cx, cy);
			glVertex2f(ax, ay);
		glEnd();
	 }
	if (three) {
	glBegin(GL_POLYGON);
		glVertex2f(ax, ay);
		glVertex2f(bx, by);
		glVertex2f(cx, cy);
	glEnd();
	}
	
	glPopMatrix();
	glutSwapBuffers();
}


//straight from textbook
//rotates poly by an angle of 'speed'
//resets if it exceeds 360 or goes below -360
void spinDisplay() {
	 spin = (spin + speed);
	 if (spin <= MAX_NEG_ANGLE|| spin >= MAX_ANGLE) spin = ZERO;
	 glutPostRedisplay();
}
//modified from textbook
//rotates poly by an angle of 'speed', subtracting to move reverse
//resets if it exceeds 360 or goes below -360
void spinReverse() {
	 spin = spin - speed;
	 if (spin <= MAX_NEG_ANGLE || spin >= MAX_ANGLE) spin = ZERO;
	 glutPostRedisplay();
}

void storePos(int x, int y) {
	start_x = x;
	start_y = y;
}

void mouseHandler(int x, int y) {
	/*
		Mouse handler for colors (change in x via mouse motion) : if it's within an acceptable intensity,
		change it; if it goes beyond, then reset it to the minimum or maximum value accordingly so as
		to ensure that the picture doesn't go all white/all black.
		Change in color is normalized to 1.
	*/
		if (red_intensity >= MIN_INTENSITY && blue_intensity >= MIN_INTENSITY && green_intensity >= MIN_INTENSITY
				&& red_intensity <= MAX_INTENSITY && blue_intensity <= MAX_INTENSITY && green_intensity <= MAX_INTENSITY) {
			red_intensity = INITIAL_INTENSITY + (float(x - home_x)/(2*width));
			blue_intensity = INITIAL_INTENSITY + (float(x - home_x)/(2*width));
			green_intensity = INITIAL_INTENSITY + (float(x- home_x)/(2*width));
		}
		if (red_intensity <= MIN_INTENSITY) {
			if (x < home_x) {
				red_intensity = MIN_INTENSITY;
			}
			else {
				red_intensity = INITIAL_INTENSITY + (float(x - home_x)/(2*width));
			}
		}
		if (green_intensity <= MIN_INTENSITY) {
			if (x < home_x) {
					green_intensity = MIN_INTENSITY;
				}
				else {
					green_intensity = INITIAL_INTENSITY + (float(x - home_x)/(2*width));
				}
		}
		if (blue_intensity <= MIN_INTENSITY) {
			if (x < home_x) {
				blue_intensity = MIN_INTENSITY;
			}
			else {
				blue_intensity = INITIAL_INTENSITY + (float(x - home_x)/(2*width));
			}
		}
		if (red_intensity >= MAX_INTENSITY) {
			if (x > home_x) {
				red_intensity = MAX_INTENSITY;
			}
			else {
				red_intensity = INITIAL_INTENSITY + (float(x - home_x)/(2*width));
			}
		}
		if (green_intensity >= MAX_INTENSITY) {
			if (x > home_x) {
					green_intensity = MAX_INTENSITY;
				}
				else {
					green_intensity = INITIAL_INTENSITY + (float(x - home_x)/(2*width));
				}
		}
		if (blue_intensity >= MAX_INTENSITY) {
			if (x > home_x) {
				blue_intensity = MAX_INTENSITY;
			}
			else {
				blue_intensity = INITIAL_INTENSITY + (float(x - home_x)/(2*width));
			}
		}

	// based on state variables, change the color
	if (cyan) {
		glClearColor(0.0,green_intensity,blue_intensity,alpha);
	}
	if (magenta) {
		glClearColor(red_intensity,0.0,blue_intensity,alpha);
	}
	if (yellow) {
		glClearColor(red_intensity,green_intensity,0.0,alpha);
	}
	if (white) {
		glClearColor(red_intensity,green_intensity,blue_intensity,alpha);
	}
	/*
		Dealing with changes to speed and direction. To make this as simple as possible,
		speed will NOT change once the mouse has left the window. Per the instructions, 
		speed is 'forgotten' once the mouse enters the RAISED state and restarts at a new
		speed each time. This means that if you click close to the bounds and move out of
		bounds, it will not appear as though any change is being made. Therefore,
		to verify that speed is in fact changing, you can uncomment line 289.

		Speed is a function of start_y, the y-coordinate of the initial click, and
		the present y-coordinate of the mouse, normalized to 1 by dividing by the
		height. Moving 'up' (which means decreasing y-values of the mouse in the window)
		means y < start_y so the normalizewwd difference is subtracted from 1. 
		Similar logic holds for moving 'down' (which increases the y-value of the mouse
		in the window). THESE ARE MOUSE COORDINATES< **NOT** OPENGL COORDINATES.
	*/

	if (mouse == LEFT || mouse == RIGHT) {
		if (y >= height) y = height;
		if (y <= ZERO) y = ZERO;
		if (speed <= MAX_SPEED && speed >= MIN_SPEED) {
			speed = 1 - (float(y - start_y))/height;
			// cout << " speed is : " << speed << endl; // uncomment for speed
		}
		if (speed >= MAX_SPEED) {
			if (abs(y - start_y) < height) {
				speed -= PUSH;
			}
			else {
				speed = MAX_SPEED;
			}
		}

		if (speed <= MIN_SPEED) {
			if (y >= ZERO && y < last_y) {
				speed += PUSH;
			}
			else {
				speed = MIN_SPEED; // if outside the normalized bounding box, stay at 0.01
			}
		spin = spin + speed;
		}
	}
	/*
		Click and drag: compute the translation vector and use a rotation
		matrix to translate the shape. I note that DRAG_NORMALIZE
		is somewhat of a magic constant; truthfully, I played around with
		numbers and that was the best one.
	*/
	if (mouse == CENTER) {
		float new_x = float(last_x - x)/DRAG_NORMALIZE;
		float new_y = float(last_y - y)/DRAG_NORMALIZE;
		glTranslatef(-1*new_x, new_y,0);
	}
	last_x = x;
	last_y = y;
	glutPostRedisplay();
}

//Keyboard sets global states and then calls glutPostRedisplay() to
// reflect those changes.
void keyboard(unsigned char key, int x, int y) {
	glClear(GL_COLOR_BUFFER_BIT);
	switch(key) {
	case C:
		glClearColor(0.0,green_intensity,blue_intensity,alpha);
		setColor(C);
		break;
	case M:
		glClearColor(red_intensity,0.0,blue_intensity,alpha);
		setColor(M);
		break;
	case Y:
		glClearColor(red_intensity,green_intensity,0.0,alpha);
		setColor(Y);
		break;
	case W:
		glClearColor(red_intensity, green_intensity, blue_intensity,alpha);
		setColor(W);
		break;
	case ONE:
			one = true;
			two = false;
			three = false;
		break;
	case TWO:
			one = false;
			two = true;
			three = false;
		break;
	case THREE:
			one = false;
			two - false;
			three = true;
		break;
	default:
		break;
	}
	glutPostRedisplay();

}

//Instructions specify that the polygon must 'forget' its speed
//after the mouse is lifted. Hence, the speed is reset 
//once the button is lifted
void resetRotation() {
	speed = 0.5;
}

void reshape(int w, int h)
{
	width = w; // so that bounds can be updated once window is resized
	height = h; // so that bounds can be updated once window is resized
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-50, 50, -50, 50, -1, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


// Mouse function that is called in main via glutMouseFunc
// Sets the mouse state (left, right, center) and then
// calls a mouse handler (named mouseHandler) which
// redirects according to the state
void mousef(int button, int state, int x, int y)
{
start_y = y;
start_x = x;

switch (button) {
	 case GLUT_LEFT_BUTTON:
		 if (state == GLUT_DOWN) {
			 glutMotionFunc(mouseHandler);
			 glutIdleFunc(spinDisplay);
			 mouse = LEFT;
		 }
		 else {
			glutIdleFunc(resetRotation);
			mouse = RAISED;
		 }
		 break;
	 case GLUT_RIGHT_BUTTON:
		 if (state == GLUT_DOWN) {
			glutMotionFunc(mouseHandler);
			glutIdleFunc(spinReverse);
			mouse = RIGHT;
		 }
		 else {
			glutIdleFunc(resetRotation);
			mouse = RAISED;
		 }
		 break;
	 case GLUT_MIDDLE_BUTTON:
		 if (state == GLUT_DOWN) {
			 glutMotionFunc(mouseHandler);
			 start_y = y;
			 start_x = x;
			 last_y = y;
			 last_x = x;
			 mouse = CENTER;
		 }
		 else {
			glutIdleFunc(NULL);
			mouse = RAISED;
			last_y = y;
			last_x = x;
		 }	
	 break;

	 default:
		 glutIdleFunc(NULL);
		break;
	}
}

int main(int argc, char** argv)
{
 glutInit(&argc, argv);
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
 glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
 glutInitWindowPosition(100, 250);
 glutCreateWindow("Sarah Sahibzada - Assignment 2");
 init();
 glutKeyboardFunc(keyboard);
 glutDisplayFunc(display);
 glutMouseFunc(mousef);
 glutReshapeFunc(reshape);
 glutMainLoop();
 return 0;
}

