#define ImageW 400
#define ImageH 400
#include <GL/glut.h>
#include <iostream>
#include <ostream>
#include <fstream>
#include <math.h>
#include <random>
#include <chrono>


#include "sorted_edge_table.h"
#pragma once

// important globals
float framebuffer[ImageH][ImageW][3];
float ZMAX = 10000.0;	// NOTE: Assume no point has a Z value greater than 10000.0
float zbuffer[ImageH][ImageW][1]; // z buffer
const float eye_vector[3] = { 0,0,-1 };
char* sourcefile = "triangle.dat";
typedef struct color {
	float r, g, b;		// Color (R,G,B values)
	color() {};
	color(float red, float green, float blue) : r(red), g(green), b(blue) {};
	
} color;
// todo: implement color math
ostream& operator<<(ostream& os, color c) {
	return os << c.r << "\t" << c.g << "\t" << c.b << endl;
}


