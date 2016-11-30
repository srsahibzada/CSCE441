/*
	SARAH SAHIBZADA
	CSCE 441 - 500
	HOMEWORK 3 : SCAN CONVERSION AND CLIPPING

	Main function, clipping rectangle struct and Sutherland-Hodgman clipping algorithm,
	and scan conversion. In order to do this, I referenced the course slides, 
	my lecture notes, and the course textbook.

*/
//#include "sorted_edge_table.h"
#include "pallette.h"
#include "sorted_edge_table.h"
#include "polygons.h"
#pragma once

// function prototypes 
void fill_frame_buffer(int,int,float,float,float);
void display(void);
void scan_convert();
void clearFramebuffer();

// shape data structures -- store the original polygons in a polygons struct, and the most recently clipped polygons in another
polygons final_list; //global variable maintaining all of the shapes for edge list purposes
polygons clipped_list;

// states
bool create_poly = true; // polygon creation state
bool mouse_down = false; // flag for mouse capture, ONLY for use with clip rect
bool clip_rectangle_created = false; // first clip rect created

// scan conversion globals
pallette pall; //global pallette, to be exhausted by a single run of the program
color active_color; // to be reset as we go along
queue<point> current_shape; // clear after each scan conversion
vector<point> all_points; // clear after each scan conversion
vector<edge> edges; // clear after each scan conversion
vector<edge> sorted_edges; // not messing with order in clipping

int num_clicks = 0;
point start_point;
int shape_max_y;
int shape_min_y;
int shape_max_x;
int shape_min_x;

typedef struct clip_rectangle {
	pallette p;
	color border_color;
	point first, second, third, fourth;
	vector<edge> outline;
	clip_rectangle() {}; // default constructor
	typedef enum{left,right,top,bottom} border; //abstraction for the sides and their order. clipping order is left, right, top, bottom
	int left_side, right_side, top_side, bottom_side; // actual values that correspond to sides in enum
	clip_rectangle(point p1, point p2) {
		//calculate points
		first = p1;
		second = point(p1.x, p2.y);
		third = p2;
		fourth = point(p2.x, p1.y);
		
		// set color of rectangle
		border_color = p.get_border_color();
		edge leftedge, rightedge, topedge, bottomedge;
		// create clipper outline
		//deal with cases where point at mouse lift is to the left or bottom of initial pt
		left_side = min(first.x,third.x);
		right_side = max(first.x,third.x);
	    bottom_side = min(first.y,third.y);
		top_side = max(first.y,third.y);

		//draw clip rectangle. we need not use DDA because lines are straight
		for (int i = bottom_side; i < top_side; i++) {
			fill_frame_buffer(first.x,i,border_color.get_red(), border_color.get_green(), border_color.get_blue());
		}
		for (int i = bottom_side; i < top_side; i++) {
			fill_frame_buffer(third.x,i,border_color.get_red(), border_color.get_green(), border_color.get_blue());
		}
		
		for (int i = left_side; i < right_side; i++) {
			fill_frame_buffer(i,first.y,border_color.get_red(), border_color.get_green(), border_color.get_blue());
		}
		for (int i = left_side; i < right_side; i++) {
			fill_frame_buffer(i,third.y,border_color.get_red(), border_color.get_green(), border_color.get_blue());
		}
		if (first < third) {
			leftedge = edge(first,second);
			rightedge = edge(third,fourth);
		}
		else {
			leftedge = edge(third,fourth);
			rightedge = edge(first,second);
		}
		if (first.compare_y(third)) {
			bottomedge = edge(first,second);
			topedge = edge(third,fourth);
		}
		else {
			bottomedge = edge(third,fourth);
			topedge = edge(first,second);
		}
	}
	
	bool inside_line(point testpt,  border bord) {
			if (bord == left && testpt.x > left_side) {
				return true;
			} 
			else if (bord == left && testpt.x < left_side) {
				return false;
			}
			else if (bord == right && testpt.x < right_side) {
				return true;
			}
			else if (bord == right && testpt.x > right_side) {
				return false;
			}
			else if (bord == bottom && testpt.y > bottom_side) {
				return true;
			}
			else if (bord == bottom && testpt.y < bottom_side) {
				return false;
			}
			else if (bord == top && testpt.y < top_side) {
					return true;
			}
			else {
				return false;
			}
	}

	bool cross_line(edge e, border bord) {
		bool a_inside = inside_line(e.a,bord);
		bool b_inside = inside_line(e.b,bord);
		bool a_outside = !inside_line(e.a,bord);
		bool b_outside = !inside_line(e.b,bord);
		if (a_inside && b_inside) {
			return false;
		}
		else if (a_outside && b_outside) {
			return false;
		}
		else {
			return true;
		}
		
	}


	point intersection_with_border(edge e, border bord) {	
		float intersect_x; // computed x intersection (may be a border)
		float intersect_y; // computed y intersection (may be a border)
		float start_x = e.a.x;
		float start_y = e.a.y;
		float end_x = e.b.x;
		float end_y = e.b.y;
		if (end_x == start_x && (bord == left|| bord == right) ) {
			return point(end_x,end_y); // just treat end point like an intersection. we can only handle this for top and bottom border
		}
		else {
			float slope = (float(start_y) - float(end_y))/(float(start_x) - float(end_x)); //switching order distorts clip
			switch(bord) {
				case left:
					intersect_x = left_side;
					intersect_y = end_y+float(left_side-end_x)*slope;
					break;
				case right:
					intersect_x = right_side;
					intersect_y = end_y+float(right_side-end_x)*slope;
					break;
				case top:
					if (start_x == end_x) { // case vertical line; don't want to divide by zero
						intersect_x = end_x;
						intersect_y = top_side;
					}
					else {
						intersect_x = end_x+float(top_side-end_y)/slope;
						intersect_y = top_side;
					}
					break;
				case bottom:
					if (start_x == end_x)  {// case vertical line
						intersect_x = end_x;
						intersect_y = bottom_side;
					}
					else {
						intersect_x = end_x+float(bottom_side-end_y)/slope;
						intersect_y = bottom_side;
					}
					break;
			}
		}
		return point(intersect_x,intersect_y); // the point where it intersects with the edge
						// we have to use points, not edges, because the edges change and
						// this saves computation
	}
	
	//pass by reference so we don't need to return a vector
	void clip_side(vector<edge>& shape, border side) {
		vector<point> final_points; // aux data structure to store points
		vector<edge> clipped_edges; // ultimately clipped edges 
		for (int i = 0; i < shape.size(); i++) {
			edge to_clip = shape[i];
			if (cross_line(to_clip,side)) {
						if (inside_line(to_clip.b,side)) {
							// if line crosses and ends inside, store intersection and inner point
							final_points.push_back(intersection_with_border(to_clip,side));
							final_points.push_back(to_clip.b);
						}
						else if (inside_line(to_clip.a,side)) {
							// if line crosses and ends outside, store intersection only
							final_points.push_back(intersection_with_border(to_clip,side));
						}
					}
					//case not crossing
					else {
						// if both points are inside the line, just store the second one
						if (inside_line(to_clip.a,side) && inside_line(to_clip.b,side)) {
							final_points.push_back(to_clip.b);
						}
						else {
							// if line does not cross and both points are not inside, then both are outside
							// do not store anything
						}
					}
					
			
			} 

		// once we're at the end of the clipping process, we need to be ready to scan convert
		// hence, if we're at the top side, we only push back non horizontal edges
		// otherwise we can push back all edges because at the end the horizontal ones are clipped
		if (side == top) {		
			for (int i = 0; i < final_points.size(); i++) {
				//cout << final_points[i];
				if (i + 1 == final_points.size()) {
					//clipped_edges.push_back(edge(final_points[i],final_points[0]));
					edge e(final_points[i],final_points[0]);
					if (!e.bad) {
						clipped_edges.push_back(e);
					}
				}
				else {
					edge e(final_points[i],final_points[i+1]);
					if (!e.bad) {
						clipped_edges.push_back(e);
					}
				}
			}
		}
		else {
			for (int i = 0; i < final_points.size(); i++) {
				if (i + 1 == final_points.size()) {
					edge e(final_points[i],final_points[0]);
					clipped_edges.push_back(e);
				}
				else {
					edge e(final_points[i],final_points[i+1]);
					clipped_edges.push_back(e);
					
				}
			}
		
		}

		shape = clipped_edges; // we pass by reference; shape is now its clipped self.
	} // end function
	
	
	// clipping and scan conversion occurs here
	void render_all() {
		for (int i = 0; i < final_list.size(); i++) {
			color active_color = final_list.color_at(i); // active color
			vector<edge> new_shape = final_list[i]; // store the shape to be clipped
			
			// sutherland hodgman algorithm: clip along left, right, bottom, top sides, respectively
			clip_side(new_shape,left);		
			clip_side(new_shape,right);		
			clip_side(new_shape,bottom);	
			clip_side(new_shape,top);
			
			if (new_shape.size() != 0) {
				clipped_list.push_back(new_shape); // vec<vec<edge>> gets a new vec<edge>
				clipped_list.add_sorted(new_shape); // add list that can be sorted
				clipped_list.push_back(active_color);
			}
		}
		clearFramebuffer();

		for (int i = 0; i < clipped_list.size(); i++) {
			sorted_edges = clipped_list[i]; // set global sorted_edges to this so we can proceed with scan_convert()

			active_color = clipped_list.color_at(i);
			int lmax_y = clipped_list[i][0].max_y; // recalculate min, max y so we have accurate shape heights
			int lmin_y = clipped_list[i][0].min_y;
			for (int j = 0; j < clipped_list[i].size(); j++) {
				if (clipped_list[i][j].min_y < lmin_y) {
					lmin_y  = clipped_list[i][j].min_y;
				}
				if (clipped_list[i][j].max_y > lmax_y) {
					lmax_y  = clipped_list[i][j].max_y;
				}
			}
			shape_min_y = lmin_y;
			shape_max_y = lmax_y;
			scan_convert(); // scan conversion of clipped shape

			// for ease of grading, keep window on screen after clipping
			for (int i = bottom_side; i < top_side; i++) {
				fill_frame_buffer(first.x,i,border_color.get_red(), border_color.get_green(), border_color.get_blue());
			}
			for (int i = bottom_side; i < top_side; i++) {
				fill_frame_buffer(third.x,i,border_color.get_red(), border_color.get_green(), border_color.get_blue());
			}
		
			for (int i = left_side; i < right_side; i++) {
				fill_frame_buffer(i,first.y,border_color.get_red(), border_color.get_green(), border_color.get_blue());
			}
			for (int i = left_side; i < right_side; i++) {
				fill_frame_buffer(i,third.y,border_color.get_red(), border_color.get_green(), border_color.get_blue());
			}
			display();

		}
	}

} clip_rectangle;

#define ImageW 400
#define ImageH 400

float framebuffer[ImageH][ImageW][3];
float buffer_with_shapes[ImageH][ImageW][3];
clip_rectangle cr;

enum mouse {notset = -1, left, right};
int mousemode = notset;

// Draws the scene
void drawit(void)
{
   glDrawPixels(ImageW,ImageH,GL_RGB,GL_FLOAT,framebuffer);
   glFlush();
}


void display(void)
{
	drawit();
}
// Clears framebuffer to black
void clearFramebuffer()
{
	int i,j;

	for(i=0;i<ImageH;i++) {
		for (j=0;j<ImageW;j++) {
			framebuffer[i][j][0] = 0.0;
			framebuffer[i][j][1] = 0.0;
			framebuffer[i][j][2] = 0.0;
		}
	}
}

void fill_frame_buffer(int x, int y, float r, float g, float b) {
	framebuffer[y][x][0] = r;
	framebuffer[y][x][1] = g;
	framebuffer[y][x][2] = b;
}
void copy_frame_buffer() {
	for (int i = 0; i < ImageH;i++) {
		for (int j = 0; j < ImageW; j++) {
			buffer_with_shapes[i][j][0] = framebuffer[i][j][0];
			buffer_with_shapes[i][j][1] = framebuffer[i][j][1];
			buffer_with_shapes[i][j][2] = framebuffer[i][j][2];
		}
	}

}

void scan_convert() {
					sorted_edge_table edge_table;
					sort(sorted_edges.begin(), sorted_edges.end(),y_less_than); // sort edges by smallest x value
					vector<edge>::iterator it = sorted_edges.begin();
					int height = shape_max_y - shape_min_y; //overall height
					int counter = shape_min_y;
					// construct the edge list
					while (counter != shape_max_y) {
						vector<edge> line; // individual line for edge list
						for (int i = 0; i < sorted_edges.size(); i++) {
							if (sorted_edges[i].min_y == counter) {
								line.push_back(sorted_edges[i]);
							}
						}
						sort(line.begin(), line.end(), x_less_than); // sort line by x
						edge_table.push_back(line);
						counter++;
					}
					//fill the shape: construct the active edge list
					vector<edge> ael; //active edge list
					for (int i = 0; i < edge_table.size(); i++) {
						//clean out ael
						for (int k = 0; k < ael.size(); k++) {
							if (ael[k].max_y <= (i + shape_min_y)) {
								int sparam = ael[k].max_y;
								ael.erase(ael.begin()+k); //erase that element if we're already done
								if (ael.size() % 2 != 0) { // there should never be an odd number of active edges-- if there are something is wrong
									for (int q = 0; q < ael.size(); q++) {
										if (sparam == ael[q].max_y) {
											ael.erase(ael.begin() + q);
										}
									}
								}
							}
						}
						//push new members onto the active edge list
						for (int j = 0; j < edge_table[i].size(); j++) {
							ael.push_back(edge_table[i][j]);
						}
						// draw scan line
						sort(ael.begin(), ael.end(), current_x_less_than);
						for (int l = 0; l < ael.size(); l += 2) {
							if (ael.size() > 1) {
								float x_min = ael[l].current_x;
								float x_max = ael[l+1].current_x;
								for (int m = x_min; m < x_max; m++) {
									fill_frame_buffer(m,i+shape_min_y,active_color.get_red(), active_color.get_green(), active_color.get_blue());
								}
								ael[l].current_x += ael[l].x_incr;
								ael[l+1].current_x += ael[l+1].x_incr;
							}
						}

				}
	}

// 'cleans' image by redrawing.
void reset_frame_buffer() {
	for (int i = 0; i < ImageH;i++) {
		for (int j = 0; j < ImageW; j++) {
		framebuffer[i][j][0] = buffer_with_shapes[i][j][0];
		framebuffer[i][j][1] = buffer_with_shapes[i][j][1];
		framebuffer[i][j][2] = buffer_with_shapes[i][j][2];
		}
	}
}
// set clipping mode; once we get an input 'c', we can't draw anymore
// get input c enables clip mode and disables draw mode
// also clear the screen so we can specify the clip rectangle
void set_clip_mode(unsigned char key, int x, int y) {
	if (key == 'c') {
		create_poly = false;
	}
	display();
}

//store vertices and call scan conversion
void store_vertex_and_scan_convert(int button, int state, int x, int y) {
	y = ImageH - 1 - y;
	if (final_list.size() != 10 && create_poly == true) {
			if (state == GLUT_DOWN) {
				point p(x,y);
				current_shape.push(p);
				all_points.push_back(p);
				//display();
				if (num_clicks == 0) {
					//starting a new shape
					pall.set_active_color();
					active_color = pall.get_active_color();
					start_point = p;
					shape_min_x = p.x;
					shape_max_x = p.x;
					shape_min_y = p.y;
					shape_max_y = p.y;
					num_clicks += 1;
				}
				if (current_shape.size() > 1) {
					if (p.x < shape_min_x) {
						shape_min_x = p.x;
					}
					if (p.y > shape_max_y) {
						shape_max_y = p.y;
					}
					if (p.y < shape_min_y) {
						shape_min_y = p.y;
					}
					edge e(current_shape.front(),p); // draw an edge from last 
					if (!e.bad) { 
						edges.push_back(e); // do not push back bad edges
						sorted_edges.push_back(e);
					}
					current_shape.pop(); // remove edge a, b becomes a
					num_clicks += 1;
				}
				if (button == GLUT_RIGHT_BUTTON || num_clicks == MAX_CLICKS) {	
					//construct an edge table
					num_clicks = 0;
	
					// draw an edge from last to first
					point last = current_shape.front();
					edges.push_back(edge(last,start_point));
					sorted_edges.push_back(edge(start_point,last));
					current_shape.pop(); // empty the current shape queue
					//initialize sorted edge table with empty lists
					scan_convert();
					display();
					num_clicks = 0;
					// push points onto final_list; this is the reference point for clipping
					final_list.push_back(all_points);
					final_list.push_back(edges);
					final_list.push_back(active_color); // will get reset in new shape
					final_list.add_sorted(sorted_edges);
					all_points.clear();
					edges.clear();
					sorted_edges.clear();
					while (!current_shape.empty()) {
						current_shape.pop(); // just in case
					}
			}
		}
		copy_frame_buffer(); // once done rendering all shapes, 'remember' screen so that we can draw the clip rectangle
}
	else if (!create_poly && !clip_rectangle_created) {
		if (button == GLUT_LEFT_BUTTON) {
			if (state == GLUT_DOWN) {
				clipped_list.clear();
				reset_frame_buffer();
				cr.first = point(x,y);
			}
			else {
				cr.render_all();
				cr = clip_rectangle();
			}
		}
	}
}

void draw_clip_rectangle(int x, int y) {
	if (!create_poly && !clip_rectangle_created && x < ImageW && x > 0 && y < ImageH && y > 0) {
		y = ImageH - 1 - y;
		point p(x,y);
		point p_origin = cr.first;
		reset_frame_buffer();
		cr = clip_rectangle(p_origin,p);
		display();
	}

}

void setFramebuffer(int x, int y, float R, float G, float B)
{
	// changes the origin from the lower-left corner to the upper-left corner
	y = ImageH - 1 - y; // added by dr. schaefer to match coordinates
	if (R<=1.0)
		if (R>=0.0)
			framebuffer[y][x][0]=R;
		else
			framebuffer[y][x][0]=0.0;
	else
		framebuffer[y][x][0]=1.0;
	if (G<=1.0)
		if (G>=0.0)
			framebuffer[y][x][1]=G;
		else
			framebuffer[y][x][1]=0.0;
	else
		framebuffer[y][x][1]=1.0;
	if (B<=1.0)
		if (B>=0.0)
			framebuffer[y][x][2]=B;
		else
			framebuffer[y][x][2]=0.0;
	else
		framebuffer[y][x][2]=1.0;
}



void init(void)
{
	clearFramebuffer();
}

int main(int argc, char** argv)
{
	
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(ImageW,ImageH);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Sarah Sahibzada - Homework 3");
	init();	
	
	glutMouseFunc(store_vertex_and_scan_convert);
	glutMotionFunc(draw_clip_rectangle);
	glutDisplayFunc(display);
	glutKeyboardFunc(set_clip_mode);
	glutMainLoop();
	return 0;
}