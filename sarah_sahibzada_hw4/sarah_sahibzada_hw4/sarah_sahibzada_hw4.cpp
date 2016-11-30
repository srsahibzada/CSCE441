#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
/*
	Sarah Sahibzada



*/


bool increasing_x = true;
bool increasing_y = true;
//coordinates for gluLookAt();
GLdouble eye_x = 0.0;
GLdouble eye_y = 0.0;
GLdouble eye_z = 0.0;

GLdouble up_x = 0.0;
GLdouble up_y = 1.0;
GLdouble up_z = 0.0;

//movement for new eye coords
GLdouble x_mvmt = 0.0;
GLdouble y_mvmt = 0.0;
GLdouble z_mvmt = -15.0;

//for zoom
GLdouble x_scale = 1.0;
GLdouble y_scale = 1.0;
GLdouble z_scale = 1.0;

int width = 800;
int height = 800;
int scale_width = width;
int scale_height = height;
double fovy = 90.0;


void reset_all();
typedef enum viewmode {NONE, PAN, ZOOM, ROTATE, TAIL, HEAD, LEGS, STAND};
viewmode current_mode = NONE;
viewmode last_mode = NONE;
// head values
int head_horizontal = 0;
int head_vertical = 0;
int leg_forward = 0;
int leg_backward = 0;

int wag = 0; 
int raise = 0;
static void bug_segment(bool is_tail) {	
	// component 
	glPushMatrix();
		glColor3f(0.0,1.0,1.0);
		if (is_tail) {
			glPushMatrix();
				glTranslatef(0.75,0,0);
				glColor3f(0.0,1.0,0.0);
				glutWireSphere(0.5,10,10);
				glColor3f(0.0,1.0,1.0);
				glRotatef((GLfloat)wag,0.0,0.0,1.0);
				glRotatef((GLfloat)raise,0.0,1.0,0.0);
				glTranslatef(-0.75,0,0);
				glutWireCube(1.0);
			glPopMatrix();
		}
		else {
	    glPushMatrix();
			glutWireCube(1.0);
			glColor3f(0.0,1.0,0.0);
			glTranslatef(0.75,0,0);
			glutWireSphere(0.5,10,10);
		glPopMatrix();
		}
		
	glPopMatrix();


	// legs -- on any body segment that isn't a tail
	// body is comprised of 3 segments and a tail
	if (!is_tail) {
		glPushMatrix();
			glColor3f(1.0,1.0,1.0);
			glRotatef((GLfloat)leg_forward,0,1,0);
			//glRotatef((GLfloat)leg_backward,0,1,0);
			glBegin(GL_LINES);
				glVertex3f(0.0,0.0,0.5);
				glVertex3f(0.0,-2.0, 1.0);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(0.0,-2.0,1.0);
				glVertex3f(0.0,-2.0,0.5);
			glEnd();

		glPopMatrix();
		glPushMatrix(); // bottom part of leg can move
			glColor3f(1.0,1.0,1.0);
			glRotatef((GLfloat)leg_forward,0,1,0);
			glBegin(GL_LINES);
				glVertex3f(0.0,0.0,-0.5);
				glVertex3f(0.0,-2.0,-1.5);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(0.0,-2.0,-1.5);
				glVertex3f(0.0,-2.0,-0.5);
			glEnd();
		glPopMatrix();
	}

}

static void bug_tail() {
	glPushMatrix();
		bug_segment(true);
	glPopMatrix();
}


void mouse_handler(int x, int y) {
	if (increasing_x) {
		if (wag >= -30 && wag <= 30) {
			wag = ((wag + x) % 60) - 30;
		}
	}
	if (wag >= 30) {
		increasing_x = false;
		wag -= 5;
	}
	
	if (! increasing_x) {
			if (wag >= -30 && wag <= 30) {
			wag = ((wag - x) % 60 )- 30;
		}
	}
		if (wag <= -30) {
				increasing_x = true;
				wag += 5;
			}
		
	if (increasing_y) {
		if (raise >= -60 && raise <= 60) {
				raise = ((raise + x) % 120) - 60;
			}
	}
	if (raise >= 60) {
		increasing_y = false;
		raise -= 5;
	}
	if (!increasing_y) {
		if (raise >= -60 && raise <= 60) {
				raise = ((raise - x) % 120)- 60;
			}
	}
	if (raise <= -60) {
		increasing_y = true;
		raise += 5;
	}
		
		glutPostRedisplay();

	
}
void mouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		reset_all();
		glutMotionFunc(mouse_handler);
		//glutPostRedisplay();
	}

}
int horizontal_neck = 0;
int vertical_neck = 0;

static void bug_head() {
	//the neck
	glPushMatrix();
		glColor3f(0.0,0.0,1.0);
		glPushMatrix();
		glRotatef((GLfloat)horizontal_neck,0.0,1.0,0.0);
		glRotatef((GLfloat)vertical_neck,0.0,0.0,1.0);
		glutWireSphere(0.25,10,10);
		//the head
		glColor3f(1.0,0.0,1.0);
		glTranslatef(1.5,0.75,0.0);
		
		glutWireSphere(1.5,10,10);
		//glTranslatef(-1.0,-0.5,0.0);

		// the eyes
		glColor3f(0.0,0.0,1.0);
		glTranslatef(1.5,0.5,0.0);
		glutSolidSphere(0.25,10,10);
		glTranslatef(0.0,0.0,-0.3);
		glutSolidSphere(0.25,10,10);
		glPopMatrix();
		//glTranslatef(0.0,0.0,0.3);
	glPopMatrix();
}
void init() {
	 glClearColor(0.0, 0.0, 0.0, 0.0);
	 glShadeModel(GL_FLAT);
	 glEnable(GL_DEPTH_TEST); // depth buffering
	 glEnable(GL_CULL_FACE);  // culling hsr
}
int bug_stand1 = 0;
int bug_stand2 = 0;
int bug_stand3 = 0;
int bug_stand4 = 0;
/*Heirarchical model: body composed of segments, head*/
void display() {
	if (current_mode != ROTATE) {
	glLoadIdentity();
	}
	glCullFace(GL_BACK);
	//glFrontFace(GL_CW);
	//gluLookAt(0,0,0,0,0,-15,0,1,0);
	gluLookAt(eye_x,eye_y,eye_z,eye_x + x_mvmt,eye_y + y_mvmt,eye_z + z_mvmt,up_x, up_y, up_z);
	glScalef(x_scale,y_scale,z_scale);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//glEnable(GL_DEPTH_TEST);
	
		//bug_segment(true);
		glPushMatrix();
		//glLoadIdentity();
			glTranslatef(-4.0,0.0,-5.0);
			glScalef(0.75,0.75,0.75); // is going to drive me crazy otherwise
			bug_tail();
			//glScalef(1.15,1.0,1.0);
			glPushMatrix();
				glTranslatef(1.5,0.0,0.0);
				glRotatef((GLfloat)bug_stand4,0,1,0);
				bug_segment(false);
				glPushMatrix();
					glTranslatef(1.5,0.0,0.0);
					glRotatef((GLfloat)bug_stand3,0,1,0);
					bug_segment(false);
				
					glPushMatrix();
						glTranslatef(1.5,0.0,0.0);
						glRotatef((GLfloat)bug_stand2,0,1,0);
						bug_segment(false);
							glPushMatrix();
								glTranslatef(1.5,0.0,0.0);
								glRotatef((GLfloat)bug_stand1,0,1,0);
								bug_segment(false);
								glTranslatef(1.25,0.3,0.0);
								bug_head();
							glPopMatrix();
						glPopMatrix();
				glPopMatrix();
			glPopMatrix();
	glPopMatrix();
		glutSwapBuffers();
}

void reshape(int w, int h) {
	 glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	 glMatrixMode(GL_PROJECTION);
	 glLoadIdentity();
	 gluPerspective(fovy, (GLfloat) w/(GLfloat) h, 1.0, 90.0);
	 glMatrixMode(GL_MODELVIEW);
	 glLoadIdentity();
	 //glTranslatef(0.0+x_mvmt, 0.0+y_mvmt, -8.0);
	 ///gluLookAt(eye_x,eye_y,eye_z,eye_x + x_mvmt, eye_y + y_mvmt, eye_z + z_mvmt,0,1,0);
	 gluLookAt(eye_x,eye_y,eye_z,eye_x + x_mvmt,eye_y + y_mvmt,eye_z + z_mvmt,up_x, up_y, up_z);
	 glScalef(x_scale,y_scale,z_scale);
	 glutPostRedisplay();
	 width = w;
	 height = h;
}

//pan functions 
void pan_left() {
	//
	if (x_mvmt >= -15) x_mvmt -= 0.05;
	glLoadIdentity();
	gluLookAt(eye_x+x_mvmt,eye_y,eye_z,eye_x + x_mvmt,eye_y + y_mvmt,eye_z + z_mvmt,up_x, up_y, up_z);
	glScalef(x_scale,y_scale,z_scale);
	glutPostRedisplay();
}

void pan_right() {
	//glScalef(x_scale,y_scale,z_scale);
	if (x_mvmt <= 15)  x_mvmt += 0.05;
	glLoadIdentity();
	gluLookAt(eye_x+x_mvmt,eye_y ,eye_z,eye_x + x_mvmt,eye_y + y_mvmt,eye_z + z_mvmt,up_x, up_y, up_z);
	glScalef(x_scale,y_scale,z_scale);
	glutPostRedisplay();
}

void pan_up() {

	//glScalef(x_scale,y_scale,z_scale);
	if (y_mvmt <= 15) y_mvmt += 0.05;
	glLoadIdentity();
	gluLookAt(eye_x+x_mvmt,eye_y ,eye_z,eye_x + x_mvmt,eye_y + y_mvmt,eye_z + z_mvmt,up_x, up_y, up_z);
	glScalef(x_scale,y_scale,z_scale);
	glutPostRedisplay();
}

void pan_down() {
	//glScalef(x_scale,y_scale,z_scale);
	if (y_mvmt >= -15) y_mvmt -= 0.05;
	glLoadIdentity();
	gluLookAt(eye_x+x_mvmt,eye_y ,eye_z,eye_x + x_mvmt,eye_y + y_mvmt,eye_z + z_mvmt,up_x, up_y, up_z);
	glScalef(x_scale,y_scale,z_scale);
	glutPostRedisplay();

}

void zoom_out() {

	/*if (z_scale < 5) z_scale += 0.05;
	glLoadIdentity();
	gluLookAt(eye_x,eye_y,eye_z,eye_x + x_mvmt,eye_y + y_mvmt,eye_z + z_mvmt,up_x, up_y, up_z);
	glScalef(x_scale,y_scale,z_scale);
	glutPostRedisplay();*/
	if (fovy <= 150) fovy += 0.5;
	glMatrixMode(GL_PROJECTION);
	//gluOrtho2D(-100+zoom_amount, 100 + zoom_amount, -100 + zoom_amount, 100 + zoom_amount);
	glLoadIdentity();
	gluPerspective(fovy, (GLfloat) width/(GLfloat) height, 1.0, 90.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye_x,eye_y,eye_z,eye_x + x_mvmt,eye_y + y_mvmt,eye_z + z_mvmt,up_x, up_y, up_z);
	glutPostRedisplay();
	//glPopMatrix();

}
void zoom_in() {
	if (fovy >= 50) fovy -= 0.5;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLfloat) width/(GLfloat) height, 1.0, 90.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye_x,eye_y,eye_z,eye_x+x_mvmt,eye_y + y_mvmt,eye_z + z_mvmt,up_x, up_y, up_z);
	glutPostRedisplay();
	glPopMatrix();
}

void object_centered_rotation_up() {
	double current_y_mvmt = y_mvmt;
	if (y_mvmt <= 5) y_mvmt += 0.05;
	glLoadIdentity();
	gluLookAt(eye_x+x_mvmt,-1*(eye_y+y_mvmt), -1*(eye_z), eye_x + x_mvmt, eye_y + current_y_mvmt, eye_z + z_mvmt, up_x, up_y, up_z);
	glScalef(x_scale,y_scale,z_scale);
	glutPostRedisplay();
}

void object_centered_rotation_down() {
	double current_y_mvmt = y_mvmt;
	if (y_mvmt >= -5) y_mvmt -= 0.05;
	//if (current_y_mvmt == y_mvmt) cout << " here is the problem " << endl;
	glLoadIdentity();
	//gluLookAt(eye_x+current_x_mvmt,eye_y+y_mvmt ,eye_z+current_z_mvmt,eye_x+current_x_mvmt,eye_y + current_y_mvmt,eye_z+curr,up_x, up_y, up_z);
	gluLookAt(eye_x+x_mvmt,-1*(eye_y+y_mvmt), -1*(eye_z), eye_x + x_mvmt, eye_y + current_y_mvmt, eye_z + z_mvmt, up_x, up_y, up_z);
	//glRotatef(3,0,1,0);
	glScalef(x_scale,y_scale,z_scale);
	glutPostRedisplay();

}

void reset_all() {

	x_mvmt = 0.0;
	y_mvmt = 0.0;
	z_mvmt = -15.0;
	fovy = 90.0;

	 glViewport(0, 0, (GLsizei) width, (GLsizei) height);
	 glMatrixMode(GL_PROJECTION);
	 glLoadIdentity();
	 gluPerspective(fovy, (GLfloat) width/(GLfloat) height, 1.0, 90.0);
	 glMatrixMode(GL_MODELVIEW);
	 glLoadIdentity();
	 //glTranslatef(0.0+x_mvmt, 0.0+y_mvmt, -8.0);
	 ///gluLookAt(eye_x,eye_y,eye_z,eye_x + x_mvmt, eye_y + y_mvmt, eye_z + z_mvmt,0,1,0);
	 gluLookAt(eye_x,eye_y,eye_z,eye_x + x_mvmt,eye_y + y_mvmt,eye_z + z_mvmt,up_x, up_y, up_z);
	 glScalef(x_scale,y_scale,z_scale);
	 glutPostRedisplay();
}
// assign modes so the arrow keys can specify motion
void keyboard(unsigned char key, int x, int y) {
	last_mode = current_mode;
	switch(key) {
	case 'z':
		current_mode = ZOOM;
		break;
	case 'r':
		current_mode = ROTATE;
		reset_all();
		break;
	case 'p':
		current_mode = PAN;
		break;
	case 'm':
		current_mode = TAIL;
		break;
	case 'h':
		current_mode = HEAD;
		break;
	case 's':
		current_mode = STAND;
		break;
	case 'l':
		current_mode = LEGS;
		break;
	default: break; // todo: fill with rotation stuff
	}
	if (last_mode == ROTATE) {
		reset_all();
	}
	if (current_mode == STAND) {
		switch(key) {
		case '1':
			cout << " here " << endl;
			if (bug_stand1 <= 30) bug_stand1 += 5;
			break;
		case '2':
			if (bug_stand2 <= 30) bug_stand2 += 5;
			break;
		case '3':
			if (bug_stand3 <= 30) bug_stand3 += 5;
			break;
		case '4':
			if (bug_stand4 <= 15) bug_stand4 += 5;
			break;
		case '5':
			if (bug_stand1 >= -30) bug_stand1 -= 5;
			break;
		case '6':
			if (bug_stand2 >= -30) bug_stand2 -= 5;
			break;
		case '7':
			if (bug_stand3 >= -30) bug_stand3 -= 5;
			break;
		case '8':
			if (bug_stand4 >= -15) bug_stand4 -= 5;
			break;
		}
		glutPostRedisplay();
	}
	
}
// panning will take place using special keys
void arrow_keys(int key, int x, int y) {
	if (current_mode == PAN) {
		switch(key) {
		case GLUT_KEY_UP:
			pan_up();
			break;
		case GLUT_KEY_DOWN:
			pan_down();
			break;
		case GLUT_KEY_LEFT:
			pan_left();
			break;
		case GLUT_KEY_RIGHT:
			pan_right();
			break;
		}
	}
	else if (current_mode == ZOOM) {
		switch(key) {
		case GLUT_KEY_UP:
			zoom_in();
			break;
		case GLUT_KEY_DOWN:
			zoom_out();
			break;
		}
	}
	else if (current_mode == ROTATE) {
		switch(key) {
			case GLUT_KEY_UP:
				object_centered_rotation_up();
				break;
			case GLUT_KEY_DOWN:
				object_centered_rotation_down();
				break;
		}
	
	}
	else if (current_mode == HEAD) {
		switch(key) {
		case GLUT_KEY_UP:
			if (vertical_neck <= 30) vertical_neck += 5;
			break;
		case GLUT_KEY_DOWN:
			if (vertical_neck >= -30) vertical_neck -= 5;
			break;
		case GLUT_KEY_RIGHT:
			if (horizontal_neck <= 60) horizontal_neck += 5;
			break;
		case GLUT_KEY_LEFT:
			if (horizontal_neck >= -60) horizontal_neck -= 5;
			break;
		}
		glutPostRedisplay();
	}
	else if (current_mode == LEGS) {
		bool forward = true;
		switch(key) {
		case GLUT_KEY_RIGHT:
			if (leg_forward >= 40) forward = false;
			if (leg_forward <= 0) forward = true;
			if (forward) {
				leg_forward += 5;
			}
			else {
				leg_forward =0 ;
			}
			break;
	
	
		}
		glutPostRedisplay();

}

}


int main(int argc, char** argv)
{
 glutInit(&argc, argv);
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
 glutInitWindowSize(800, 800);
 glutInitWindowPosition(100, 100);
 glutCreateWindow(argv[0]);
 init();
 glutDisplayFunc(display);
 glutReshapeFunc(reshape);
 glutKeyboardFunc(keyboard);
 glutSpecialFunc(arrow_keys);
 glutMouseFunc(mouse);
 glutMainLoop();
 return 0;
}