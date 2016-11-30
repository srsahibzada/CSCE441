#include "vertex.h"
#include <algorithm>

struct edge {
	float max_y;
	float min_y;
	float max_x;
	float min_x;
	float current_x; // for scan conv
	float x_incr;
	// for z buffer
	float z_incr; // this is the initial z value at a point
	float current_z;
	//for normal interpolation/phong shading
	float current_f[3]; // normal equation
	float f_incr[3]; // increment array

	vertex v1;
	vertex v2;
	bool bad; // case horizontal
	edge(vertex v1, vertex v2) {
		if (v1.y != v2.y) {
			bad = false;
			max_y = max(v1.y, v2.y);
			min_y = min(v1.y, v2.y);
			max_x = max(v1.x, v2.x);
			min_x = min(v1.x, v2.x);
			if (v1.y < v2.y) { // if v2 is higher than v1 (y coordinate)
				current_x = v1.x;
				current_z = v1.z;
				current_f[0] = v1.nx;
				current_f[1] = v1.ny;
				current_f[2] = v1.nz;
				float f_incr_magnitude = 1 / (max_y - min_y);
				f_incr[0] = (v2.nx - v1.nx) * f_incr_magnitude;
				f_incr[1] = (v2.ny - v1.ny) * f_incr_magnitude;
				f_incr[2] = (v2.nz - v1.nz) * f_incr_magnitude;
			}
			else {
				// case where v2 is lower than v1, so we start at v2
				// set current x and z to be those of the lower one, v2
				current_x = v2.x; // v2 is the smaller one so we start there
				current_z = v2.z;
				// set f buffer for normal interpolation
				// current f is "data value at endpoint with lowest y-value" according to slides
				current_f[0] = v2.nx;
				current_f[1] = v2.ny;
				current_f[2] = v2.nz;
				float f_incr_magnitude = 1 / (max_y - min_y); // to scale fIncr
				//fIncr = (f0 - f1)/(y0 - y1) -> 1/(y0 - y1) * [f0] - [f1]
				f_incr[0] = (v1.nx - v2.nx) * f_incr_magnitude;
				f_incr[1] = (v1.ny - v2.ny) * f_incr_magnitude;
				f_incr[2] = (v1.nz - v2.nz) * f_incr_magnitude;
			}
			//current_x = min_x;
			x_incr = 1. / ((float(v2.y) - float(v1.y)) / (float(v2.x) - float(v1.x)));
			// set f_incr array
			
		}
		else {
			//case horizontal line
			bad = true;
			max_y = max(v1.y, v2.y);
			min_y = min(v1.y, v2.y);
			max_x = max(v1.x, v2.x);
			min_x = min(v1.x, v2.x);
			if (v1.y < v2.y) {
				current_x = v1.x;
				current_z = v1.z;
				current_f[0] = v1.nx;
				current_f[1] = v1.ny;
				current_f[2] = v1.nz;
			}
			else {
				current_x = v2.x;
				current_z = v2.z;
				current_f[0] = v2.nx;
				current_f[1] = v2.ny;
				current_f[2] = v2.nz;
			}
			x_incr = 0;
		}
		this->v1 = v1;
		this->v2 = v2;
		this->z_incr = (v2.z - v1.z) / (v2.y - v1.y); // compute change in y to update 
	}

};


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
	return out << "(" << e.v1.x << "," << e.v1.y << ") to (" << e.v2.x << "," << e.v2.y << ") (min_y = " << e.min_y << ")" << endl;
}

void increment_f_buffer(edge e1) {
	for (int i = 0; i < 3; i++) {
		e1.current_f[i] += e1.f_incr[i];
	}
}