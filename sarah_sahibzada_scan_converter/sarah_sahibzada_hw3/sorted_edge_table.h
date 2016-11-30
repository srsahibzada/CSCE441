/*
	SARAH SAHIBZADA
	CSCE 441 - 500
	HOMEWORK 3 : SCAN CONVERSION AND CLIPPING

	This is the sorted edge table, the fundamental data structure
	for scan line conversion. It is simply a 2D vector:
	a vector of edge vectors. This data structure is relied
	upon for the scan conversion algorithm used in sarah_sahibzada_hw3.cpp.
*/

#include "edge.h"
#include "color.h"
class sorted_edge_table {
	vector<vector<edge>> tab;
	public:
		void push_back(vector<edge> l) {
			tab.push_back(l);
		}
		vector<vector<edge>>& get_table() {
			return tab;
		}	
		vector<edge>& operator[](int idx) {
			return tab[idx];
		}
		int size() {
			return tab.size();
		}
		
};

// for debugging
ostream& operator<<(ostream& out, sorted_edge_table st) {
	vector<vector<edge>>& tab = st.get_table();
	for (int i = 0; i < tab.size(); i++) {
		out << i << ": ";
		vector<edge> ll = tab[i];
		for (vector<edge>::iterator it = ll.begin(); it != ll.end(); ++it) {
			out << *it << " , ";
		}
		out << endl;
	}
	return out;
}