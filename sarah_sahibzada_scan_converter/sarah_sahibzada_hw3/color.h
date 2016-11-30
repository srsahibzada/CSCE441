/*
	SARAH SAHIBZADA
	CSCE 441 - 500
	HOMEWORK 3 : SCAN CONVERSION AND CLIPPING
	Simple color struct that we can build a pallette off of.
	Assignment requires that each shape be a different color.

*/
#include <random>
#pragma once
struct color {
private:
	float r, g, b;		// Color (R,G,B values)
public:
	color() : r(0.0), g(0.0), b(0.0) {}
	color(float red, float green, float blue) : r(red), g(green), b(blue) {}
	float get_red() { return this->r;}
	float get_green() { return this->g;}
	float get_blue() { return this->b;}

};

