/*
	SARAH SAHIBZADA
	CSCE 441 - 500
	HOMEWORK 3 : SCAN CONVERSION AND CLIPPING

	Edge is a data structure maintaining the information about
	a line between any two points. The boolean 'bad' member
	is used by the sorted edge table in order to determine
	whether or not something can be put on the active edge list.

	Because the sorted edge table relies upon edges
	being sorted in various different ways, there are
	a number of function objects that accomplish this, 
	as well as overloaded operators.

*/
#include "point.h"
#pragma once
typedef struct edge {
	int max_y;
	int min_y;
	int max_x;
	int min_x;
	float current_x; // for scan conversion
	double x_incr; // 1/slope. assume no horiz. edges
	point a, b; //bad naming; "from point a to point b"
	bool bad;
	edge(point a, point b) {
		if (a.y != b.y){
			//must accommodate for non-horizontal edges
			bad = false;
			max_y = max(a.y,b.y);
			min_y = min(a.y,b.y);
			max_x = max(a.x,b.x);
			min_x = min(a.x,b.x);
			if (a.y < b.y) {
				current_x = a.x;
			}
			else {
				current_x = b.x;
			}
			//current_x = min_x;
			x_incr = 1./((float(b.y) - float(a.y))/(float(b.x) - float(a.x)));
		}
		else {
			//case horizontal line
			bad = true;
			max_y = max(a.y,b.y);
			min_y = min(a.y,b.y);
			max_x = max(a.x,b.x);
			min_x = min(a.x,b.x);
			if (a.y < b.y) {
				current_x = a.x;
			}
			else {
				current_x = b.x;
			}
			x_incr = 0;
		}
		this->a = a;
		this->b = b;
	}
	edge() {};
} edge;


// FUNCTION OBJECTS: USED FOR SORTING THE EDGES IN SORTED_EDGE_TABLE

bool y_equals(edge a, edge b) {
	return (a.min_y == b.min_y);
}

//create the buckets, sorted on y min
struct y_less_than {
	bool operator() (edge a, edge b) {
		return a.min_y < b.min_y;
	}
} y_less_than;

//by the time we get here, we are already sorted
//by ymin. all of them have the same ymin.
//so we sort on all xmin; if they are attached (a corner)
//then we go with the smaller max to sort it
struct x_less_than {
	bool operator() (edge a, edge b) {
		if (a.min_x != b.min_x) {
			return a.min_x < b.min_x;
		}
		else {
			return a.max_x < b.max_x;
		}
	}
} x_less_than;

struct current_x_less_than {
	bool operator() (edge a, edge b) {
		return a.current_x < b.current_x;
	}
} current_x_less_than;


// output operator for debugging purposes
ostream& operator<<(ostream& out, edge e) {
		return out << "(" << e.a.x << "," << e.a.y << ") to (" << e.b.x << "," <<  e.b.y << ") (min_y = " << e.min_y << ")" <<endl;
}