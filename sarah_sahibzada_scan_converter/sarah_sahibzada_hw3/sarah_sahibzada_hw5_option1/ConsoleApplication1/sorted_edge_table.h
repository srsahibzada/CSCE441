#include <vector>
#include "edge.h"
struct sorted_edge_table {
	vector<vector<edge>> table;
	void push_back(vector<edge> l) {
		table.push_back(l);
	}
	vector<vector<edge>>& get_table() {
		return table;
	}
	vector<edge>& operator[](int idx) {
		return table[idx];
	}
	int size() {
		return table.size();
	}
};

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