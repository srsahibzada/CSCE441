#include <ostream>
#include <iostream>

using namespace std;


struct vertex {
	float x, y, z;		// x, y, z coordinates
	float nx, ny, nz;		// Normal at the vertex
	float u, v;			// Texture coordinates

	vertex(float x, float y, float z) : x(x), y(y), z(z) {};
	vertex() : x(0), y(0), z(0) {};
	vertex(float x, float y) : x(x), y(y) {};

	friend ostream& operator<< (ostream& o, const vertex v) {
		return o << "(" << v.x << "," << v.y << "," << v.z << ")" << endl << "normals: " << "(" << v.nx << "," << v.ny << "," << v.nz << ")" << endl;
	}


};

struct barycentric_vertex : public vertex {
	barycentric_vertex(float x, float y) : vertex(x, y) {};
	friend ostream& operator<< (ostream& o, const barycentric_vertex v) {
		return o << "(" << v.x << "," << v.y << ")" << endl ;
	}

};


