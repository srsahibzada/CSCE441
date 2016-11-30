/*
	SARAH SAHIBZADA
	CSCE 441 - 500
	HOMEWORK 3 : SCAN CONVERSION AND CLIPPING

	Polygons is a data structure that holds all of the polygons that have been rendered.
	In sarah_sahibzada_hw3.cpp we maintain two of these lists: the originally drawn
	polygons and the most recently clipped ones.

*/

#include "edge.h"
struct polygons {
	vector <vector<edge>> edgevec;
	vector <vector<edge>> sortededgevec;
	vector <vector<point>> pointvec;
	vector<color> colorvec;
	int index;
	public:
		void push_back(vector<edge> ve) {
			edgevec.push_back(ve);
		}
		void push_back(vector<point> pe) {
			pointvec.push_back(pe);
		}
		void push_back(color c) {
			colorvec.push_back(c);
		}
		void add_sorted( vector<edge> sve) {
			sortededgevec.push_back(sve);
		}
		vector<edge>& operator[] (int index) {
			return edgevec[index];
		}
		color color_at(int index) {
			if (index < colorvec.size()) {
				return colorvec[index];
			}
			else {
				return color(1.0,1.0,1.0);
			}
		}
		void clear() {
			edgevec.clear();
			pointvec.clear();
			colorvec.clear();
		}
		int size() {
			//return edgevec.size();
			if (edgevec.size() == colorvec.size() && colorvec.size() ) return edgevec.size();
			else return -1;
		}
};
