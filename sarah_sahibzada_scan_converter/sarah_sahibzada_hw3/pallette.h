
/*
	SARAH SAHIBZADA
	CSCE 441 - 500
	HOMEWORK 3 : SCAN CONVERSION AND CLIPPING
	Assignment specifications state that each shape must be rendered
	in a different color; this pallette struct randomly selects one of
	a set of colors and deletes them from the instance of pallette in 
	order to prevent repeats.
*/

#include "color.h"
#include "global.h"
typedef struct pallette {
	private:
		vector<color> colors;
		color active_color;
		color clip_rectangle_color; // constant color to make it clear where cr is
	public:
		//
		pallette() {
			colors.push_back(color(230./MAX_RED, 230./MAX_GREEN, 250./MAX_BLUE));
			colors.push_back(color(0./MAX_RED, 19./MAX_GREEN, 255./MAX_BLUE));
			colors.push_back(color(75./MAX_RED, 0./MAX_GREEN, 130./MAX_BLUE));
			colors.push_back(color(138/MAX_RED, 43./MAX_GREEN, 226./MAX_BLUE));
			colors.push_back(color(106./MAX_RED, 90./MAX_GREEN, 205./MAX_BLUE));
			colors.push_back(color(255./MAX_RED, 192./MAX_GREEN, 203./MAX_BLUE));
			colors.push_back(color(255./MAX_RED, 105./MAX_GREEN, 180./MAX_BLUE));
			colors.push_back(color(219./MAX_RED, 112./MAX_GREEN, 147./MAX_BLUE));
			colors.push_back(color(119./MAX_RED, 21./MAX_GREEN, 133./MAX_BLUE));
			colors.push_back(color(255./MAX_RED, 0./MAX_GREEN, 255./MAX_BLUE));
			colors.push_back(color(128./MAX_RED, 0./MAX_GREEN, 0./MAX_BLUE));
			colors.push_back(color(170./MAX_RED, 170./MAX_GREEN, 170./MAX_BLUE));
			colors.push_back(color(113./MAX_RED, 113./MAX_GREEN, 198./MAX_BLUE));
			colors.push_back(color(56./MAX_RED, 142./MAX_GREEN, 142./MAX_BLUE));
			colors.push_back(color(198./MAX_RED, 113./MAX_GREEN, 113./MAX_BLUE));
			colors.push_back(color(179./MAX_RED, 238./MAX_GREEN, 58./MAX_BLUE));
			clip_rectangle_color = color(205./MAX_RED,64./MAX_GREEN,0./MAX_BLUE);
		}

		//i'm 1000% sure there's a better/ more object oriented way to do this
		void set_active_color() {
			srand(time(NULL));
			int which = rand() % (colors.size());
			active_color = colors[which];
			colors.erase(colors.begin() + which);
		}
		color get_active_color() {
			return this->active_color;
		}
		color get_border_color() {
			return this->clip_rectangle_color;
		}
} pallette;