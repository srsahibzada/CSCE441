#include "globals.h"
#pragma once
struct triangle {
	int whichtexture;	// The index number of the corresponding texture to apply
	// Note: Use the color returned by the texture for the
	// ambient, diffuse, and specular color, scaled by the
	// coefficients of ambient, diffuse, and specular reflection
	vertex v[3];		// The three vertices
	triangle(vertex v1, vertex v2, vertex v3) {
		v[0] = v1;
		v[1] = v2;
		v[2] = v3;
		for (int i = 0; i < 4; i++) {
			this->plane_equation[i] = 0;
		}
	}
	triangle() {}; // default constructor
	vector<edge> edges;
	int shape_max_y;
	int shape_min_y;
	float plane_equation[4];
	float kamb;			// The coefficient of ambient reflection
	float kdiff;		// The coefficient of diffuse reflection
	float kspec;		// The coefficient of specular reflection
	int shininess;		// The exponent to use for Specular Phong Illumination
	//color c; // just to test
	int num_good_edges = 0;
	int get_num_good_edges() {
		for (int i = 0; i < edges.size(); i++) {
			if (!edges[i].bad) this->num_good_edges++;
		}
		return this->num_good_edges;
	}
};

ostream& operator<<(ostream& o, triangle t) {
	o << t.plane_equation[0] << "x + " << t.plane_equation[1] << " y + " << t.plane_equation[2] << "z " << endl;
	/*for (int i = 0; i < t.edges.size(); i++) {
		o << t.edges[i] << ",";
	}*/
	/*for (int i = 0; i < 3; i++) {
		cout << t.v[i] << "\t";
	}*/
	//o << t.whichtexture<< endl;
	return o;
	
}