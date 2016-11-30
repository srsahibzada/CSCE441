/*
	SARAH SAHIBZADA
	CSCE 441 - 500
	HOMEWORK 3 : SCAN CONVERSION AND CLIPPING
	Point is an abstraction for a coordinate in the main window.
	We use points to construct edges; these are the 
	fundamental building block of this program.
*/

#pragma once
#include <ostream>
#include "global.h"

using namespace std;

typedef struct point {
	int x;
	int y;
	point(): x(0), y(0) {}
	point(int x, int y) : x(x), y(y) {}
	point& operator=(const point& source) {
		this->x = source.x;
		this->y = source.y;
		return *this;
	}
	bool compare_y(point p) {
		return this->y < p.y;
	}


} point;

//for output verification purposes
ostream& operator<<(ostream& out, point p) {
		return out << "(" << p.x << " , " << p.y << ")";
	}

bool operator<(point const& lhs, point const& rhs) {
		return lhs.x < rhs.x;
	}

bool operator>(point const& lhs, point const& rhs) {
		return lhs.x > rhs.x;
	}

bool operator==(point const& lhs, point const& rhs) {
	return ((lhs.x == rhs.x) && (lhs.y == rhs.y));
}

bool operator!=(point const& lhs, point const& rhs) {
	return ((lhs.x != rhs.x) || (lhs.y != rhs.y));
}
