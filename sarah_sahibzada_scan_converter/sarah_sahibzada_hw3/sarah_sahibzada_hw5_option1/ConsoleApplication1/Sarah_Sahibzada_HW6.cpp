#include "globals.h"
#include "triangle.h"
#define SHAPE_SIDES 3


void display();
float dot(float, float, float, float, float, float);
void normalize(float&, float&);




// the more OO way to do this would be to have used edges
void compute_plane_equation(vertex* vertices, triangle& t) {
	// compute the auxiliary vectors

	float aux_vec1[3];
	float aux_vec2[3];
	
	// compute first vector
	aux_vec1[0] = vertices[0].x - vertices[1].x;
	aux_vec1[1] = vertices[0].y - vertices[1].y;
	aux_vec1[2] = vertices[0].z - vertices[1].z;
	/*for (int i = 0; i < 3; i++) {
	cout << aux_vec1[i] << "\t";
	}
	cout << endl;*/

	// compute second vector
	aux_vec2[0] = vertices[2].x - vertices[1].x;
	aux_vec2[1] = vertices[2].y - vertices[1].y;
	aux_vec2[2] = vertices[2].z - vertices[1].z;

	// cross product
	t.plane_equation[0] = (aux_vec1[1] * aux_vec2[2]) - (aux_vec1[2] * aux_vec2[1]); // a term
	t.plane_equation[1] = -1 * ((aux_vec1[0] * aux_vec2[2]) - (aux_vec1[2] * aux_vec2[0])); // b term
	t.plane_equation[2] = (aux_vec1[0] * aux_vec2[1]) - (aux_vec1[1] * aux_vec2[0]); // c term


	// vector from shape to eye is < 0 0 -1 > therefore we need to make sure the normal
	// has negative z component

	if (t.plane_equation[2] > 0) {

		aux_vec2[0] = -1 * (vertices[0].x - vertices[1].x);
		aux_vec2[1] = -1 * (vertices[0].y - vertices[1].y);
		aux_vec2[2] = -1 * (vertices[0].z - vertices[1].z);

		// compute second vector
		aux_vec1[0] = vertices[2].x - vertices[1].x;
		aux_vec1[1] = vertices[2].y - vertices[1].y;
		aux_vec1[2] = vertices[2].z - vertices[1].z;
		
		// cross product
		t.plane_equation[0] = (aux_vec1[1] * aux_vec2[2]) - (aux_vec1[2] * aux_vec2[1]); // a term
		t.plane_equation[1] = -1 * ((aux_vec1[0] * aux_vec2[2]) - (aux_vec1[2] * aux_vec2[0])); // b term
		t.plane_equation[2] = (aux_vec1[0] * aux_vec2[1]) - (aux_vec1[1] * aux_vec2[0]); // c term


	}
	
}


struct light {
	// Note: assume all lights are white
	float x, y, z;		// x, y, z coordinates of light
	color brightness;	// Level of brightness of light (0.0 - 1.0)
};
ostream& operator<<(ostream& o, light l) {
	return  o << l.brightness << " at (" << l.x << ", " << l.y << ", " << l.z << ")" << endl;
}

struct texture {
	// Note access using getTextureRGB provided below
	int xsize, ysize;	// The size of the texture in x and y
	float* elements;	// RGB values
};


int numtriangles;		// The number of triangles in the scene
int numlights;			// The number of lights (not including ambient) in the scene
int numtextures;		// The number of textures used in the scene

color ambientlight;		// The coefficient of ambient light

triangle* trianglelist;	// Array of triangles
light* lightlist;		// Array of lights
texture* texturelist;	// Array of textures

/* Pass in a pointer to the texture, t, and the texture coordinates, u and v
Returns (in R,G,B) the color of the texture at those coordinates */
void getTextureRGB(texture* t, float u, float v, float& R, float& G, float& B) {
	int xval, yval;
	if (u<1.0)
	if (u >= 0.0) xval = (int)(u*t->xsize);
	else xval = 0;
	else xval = t->xsize - 1;
	if (v<1.0)
	if (v >= 0.0) yval = (int)(v*t->ysize);
	else yval = 0;
	else yval = t->ysize - 1;

	R = t->elements[3 * (xval*t->ysize + yval)];
	G = t->elements[(3 * (xval*t->ysize + yval)) + 1];
	B = t->elements[(3 * (xval*t->ysize + yval)) + 2];
}
// compute barycentric vertex from a vertex
barycentric_vertex compute_uv(vertex v, triangle t, texture tex) {
	float xs[3];
	float ys[3];
	float zs[3];
	for (int i = 0; i < 3; i++) {
		xs[i] = t.v[i].x;
		ys[i] = t.v[i].y;
		zs[i] = t.v[i].z;
	}
	float to_convert_x = v.x;
	float to_convert_y = v.y;
	float lambdas[3];
	

	// wikipedia algorithm for barycentric coordinates
	//https://en.wikipedia.org/wiki/Barycentric_coordinate_system
	float to_divide = (((ys[1] - ys[2])*(xs[0] - xs[2])) + ((xs[2] - xs[1])*(ys[0] - ys[2])));
	lambdas[0] = (((ys[1] - ys[2])*(to_convert_x - xs[2])) + ((xs[2] - xs[1])*(to_convert_y - ys[2]))) / to_divide;
	lambdas[1] = (((ys[2] - ys[0])*(to_convert_x - xs[2])) + ((xs[0] - xs[2])*(to_convert_y - ys[2]))) / to_divide;
	lambdas[2] = 1 - lambdas[0] - lambdas[1];
	//lambdas[0]

	
	//normalize(lambdas[0], lambdas[1]);

	return barycentric_vertex(lambdas[0],lambdas[1]);

	

}
// Draws the scene
void drawit(void)
{
	glDrawPixels(ImageW, ImageH, GL_RGB, GL_FLOAT, framebuffer);
	glFlush();
}

void initZBuffer() {
	for (int i = 0; i < ImageW; i++) {
		for (int j = 0; j < ImageH; j++) {
			zbuffer[i][j][0] = ZMAX;
		}
	}
}

void setZBuffer(int x, int y, float depth) {
	
	zbuffer[x][y][0] = depth;
}
// Sets pixel x,y to the color RGB
void setFramebuffer(int y, int x, float R, float G, float B)
{
	//y = ImageH - 1 - y;
	if (R <= 1.0)
	if (R >= 0.0)
		framebuffer[x][y][0] = R;
	else
		framebuffer[x][y][0] = 0.0;
	else
		framebuffer[x][y][0] = 1.0;
	if (G <= 1.0)
	if (G >= 0.0)
		framebuffer[x][y][1] = G;
	else
		framebuffer[x][y][1] = 0.0;
	else
		framebuffer[x][y][1] = 1.0;
	if (B <= 1.0)
	if (B >= 0.0)
		framebuffer[x][y][2] = B;
	else
		framebuffer[x][y][2] = 0.0;
	else
		framebuffer[x][y][2] = 1.0;
}
// because it's easier to pass in a color object
void setFramebuffer(int y, int x, color c)
{
	y = ImageH - 1 - y;
	float R = c.r;
	float G = c.g;
	float B = c.b;
	if (R <= 1.0)
	if (R >= 0.0)
		framebuffer[x][y][0] = R;
	else
		framebuffer[x][y][0] = 0.0;
	else
		framebuffer[x][y][0] = 1.0;
	if (G <= 1.0)
	if (G >= 0.0)
		framebuffer[x][y][1] = G;
	else
		framebuffer[x][y][1] = 0.0;
	else
		framebuffer[x][y][1] = 1.0;
	if (B <= 1.0)
	if (B >= 0.0)
		framebuffer[x][y][2] = B;
	else
		framebuffer[x][y][2] = 0.0;
	else
		framebuffer[x][y][2] = 1.0;
}

// Normalizes the vector passed in
void normalize(float& x, float& y, float& z) {
	float temp = sqrt(x*x + y*y + z*z);
	if (temp > 0.0) {
		x /= temp;
		y /= temp;
		z /= temp;
	}
	else {
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}
}
void normalize(float& x, float& y) {
	float temp = sqrt(x*x + y*y);
	if (temp > 0.0) {
		x /= temp;
		y /= temp;
		//z /= temp;
	}
	else {
		x = 0.0;
		y = 0.0;
		//z = 0.0;
	}
}

// Returns dot product of two vectors
float dot(float x1, float y1, float z1, float x2, float y2, float z2) {
	return (x1*x2 + y1*y2 + z1*z2);
}

// Returns angle between two vectors (in radians)
float angle(float x1, float y1, float z1, float x2, float y2, float z2) {
	normalize(x1, y1, z1);
	normalize(x2, y2, z2);
	return  acos(dot(x1, y1, z1, x2, y2, z2));
}


// This is an almost direct paste from my assignment 3.
// Dr. Chai has said it was OK to do this because it's still my own work.
void scan_convert(triangle t) {
	//set up useful variables that are used in loops
	// save cost of creating arrays by creating them once and using memcpy()
	sorted_edge_table tab;
	//t.c = color(red, green, blue);
	float plane_a = t.plane_equation[0];
	float plane_c = t.plane_equation[2];
	
	float diffuse_lights[3];
	float specular_lights[3];

	int x_min;
	int x_max;
	float z_val;
	float y_val;
	float start_x_incr;
	float end_x_incr;

	float f_start[3];
	float f_end[3];
	int text_id = t.whichtexture;

	vector<edge>::iterator it = t.edges.begin();
	int height = t.shape_max_y - t.shape_min_y;
	int counter = t.shape_min_y;
	while (counter != t.shape_max_y) {
		vector<edge> line; 
		for (int i = 0; i < t.edges.size(); i++) {
			if ((int)t.edges[i].min_y == counter) {
				line.push_back(t.edges[i]);
				//cout << t.edges[i] << endl;

			}
		}
		sort(line.begin(), line.end(), x_less_than);
		tab.push_back(line);
		counter++;
	
	}
	vector<edge> ael; //active edge list
	for (int i = 0; i < tab.size(); i++) {
		//clean out ael
		for (int k = 0; k < ael.size(); k++) {
			if (ael[k].max_y <= (i + t.shape_min_y)) {
				int sparam = ael[k].max_y;
				ael.erase(ael.begin() + k); //erase that element if we're already done
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
		for (int j = 0; j < tab[i].size(); j++) {			
				ael.push_back(tab[i][j]);
		}
		// draw scan line
		sort(ael.begin(), ael.end(), current_x_less_than);
		for (int l = 0; l < ael.size(); l += 2) {		
			if (ael.size() > 1) {
				x_min = ael[l].current_x;
				x_max = ael[l + 1].current_x;
				z_val = ael[l].current_z;
				// f buffer stuff for normal interpolation
				memcpy(f_start, ael[l].current_f, 3 * sizeof(float));
				memcpy(f_end, ael[l + 1].current_f, 3 * sizeof(float));
				start_x_incr = ael[l].x_incr;
				end_x_incr = ael[l + 1].x_incr;
				
				
				for (int x_val = x_min; x_val < x_max; x_val ++) {
					z_val = z_val - (plane_a / plane_c); // compute new z

					//set ambient light
					float triangle_light[3] = { t.kamb*ambientlight.r, t.kamb*ambientlight.g, t.kamb*ambientlight.b }; // set ambient pixels
					
					// compute textures
					float rgb[3];
					barycentric_vertex bv = compute_uv(vertex(x_val, i + t.shape_min_y), t, texturelist[text_id]);
					getTextureRGB(&texturelist[text_id], bv.x, bv.y, rgb[0], rgb[1], rgb[2]);
					
					// compute interpolated normal
					float df_scale = f_start[0] / (end_x_incr - start_x_incr); 
					float df[3];
					float n[3]; //initialize normal to these values
					//{ f_start[0], f_start[1], f_start[2] };
					memcpy(n, f_start, sizeof(float)*3);
					for (int i = 0; i < 3; i++) {
						n[i] += (df_scale * (f_end[i] - f_start[i]));
					}
					normalize(n[0], n[1], n[2]);


					// compute light from every light source at each pixel.
					for (int i = 0; i < numlights; i++) {
						float brightness[3] = { lightlist[i].brightness.r, lightlist[i].brightness.g, lightlist[i].brightness.b };
						memcpy(diffuse_lights, brightness, sizeof(float)* 3);
						memcpy(specular_lights, brightness, sizeof(float)* 3);

						float x_component = lightlist[i].x - x_val; // plane x coord to light x coord
						float y_component = lightlist[i].y - (i + t.shape_min_y); // plane y coord to light y coord
						float z_component = lightlist[i].z - z_val; // plane z coord to light z coord
						float l[3] = { x_component, y_component, z_component }; // vector from surface to light
						normalize(l[0], l[1], l[2]);
						float reflected[3]; // for specular
						
						float dot_product = dot(l[0], l[1], l[2], n[0], n[1], n[2]);	
						
						for (int i = 0; i < 3; i++) {
							diffuse_lights[i] *= (dot_product * t.kdiff);
						}

						// compute specular light
						float specular_scale = 2 * dot_product;
						for (int j = 0; j < 3; j++) {
							reflected[j] = (specular_scale * n[j]) - l[j];
						}
						float specular_term = t.kspec * pow(dot(reflected[0], reflected[1], reflected[2], eye_vector[0], eye_vector[1], eye_vector[2]),t.shininess);
						specular_lights[0] = lightlist[i].brightness.r * specular_term;
						specular_lights[1] = lightlist[i].brightness.g * specular_term;
						specular_lights[2] = lightlist[i].brightness.b * specular_term;
						
						
						// add scaled lights (each term accommodates Cr, Cb, Cg, so it is equivalent to adding them up and multiplying by each Ci)
						for (int i = 0; i < 3; i++) {
							triangle_light[i] += diffuse_lights[i];
							triangle_light[i] += specular_lights[i];
						}
					}
					
					// draw line based on z buffer
					if (z_val <= zbuffer[x_val][i + t.shape_min_y][0]) {
						setFramebuffer(x_val, i + t.shape_min_y, triangle_light[0]*rgb[0],triangle_light[1]*rgb[1],triangle_light[2]*rgb[2]);
						setZBuffer(x_val, i + t.shape_min_y, z_val);
					}
				}
				
				ael[l].current_x += start_x_incr;
				ael[l + 1].current_x += end_x_incr;
				ael[l].current_z += ael[l].z_incr;
				increment_f_buffer(ael[l]);
				increment_f_buffer(ael[l + 1]);
				memcpy(f_start, ael[l].current_f, 3 * sizeof(float));
				memcpy(f_end, ael[l + 1].current_f, 3 * sizeof(float));
			
			}
		}

	}
}



void display(void)
{
	for (int i = 0; i < numtriangles; i++) {
		scan_convert(trianglelist[i]); // z buffer and scan conversion
	}
	drawit();

	
}

void init(void)
{
	int i, j, k;
	initZBuffer();
	// Initialize framebuffer to clear
	for (i = 0; i < ImageH; i++) {
		for (j = 0; j < ImageW; j++) {
			framebuffer[i][j][0] = 0.0;
			framebuffer[i][j][1] = 0.0;
			framebuffer[i][j][2] = 0.0;
		}
	}

	// Load in data
	ifstream infile(sourcefile);
	if (!infile) {
		cout << "Error! Input file " << sourcefile << " does not exist!" << endl;
		exit(-1);
	}
	infile >> numtriangles >> numlights >> numtextures;

	// First read triangles
	trianglelist = new triangle[numtriangles];
	for (i = 0; i < numtriangles; i++) {
		infile >> trianglelist[i].whichtexture;
		infile >> trianglelist[i].kamb >> trianglelist[i].kdiff >> trianglelist[i].kspec;
		infile >> trianglelist[i].shininess;
		for (j = 0; j < 3; j++) {
			infile >> trianglelist[i].v[j].x >> trianglelist[i].v[j].y >> trianglelist[i].v[j].z;
			infile >> trianglelist[i].v[j].nx >> trianglelist[i].v[j].ny >> trianglelist[i].v[j].nz;
			infile >> trianglelist[i].v[j].u >> trianglelist[i].v[j].v;
		}

		edge e1(trianglelist[i].v[0], trianglelist[i].v[1]);
		edge e2(trianglelist[i].v[1], trianglelist[i].v[2]);
		edge e3(trianglelist[i].v[0], trianglelist[i].v[2]);
		if (!e1.bad) trianglelist[i].edges.push_back(e1);
		if (!e2.bad) trianglelist[i].edges.push_back(e2);
		if (!e3.bad) trianglelist[i].edges.push_back(e3);
		sort(trianglelist[i].edges.begin(), trianglelist[i].edges.end(), y_less_than); // sort edges on y

		// set min, max y value to get shape height
		float min_y = 1000000;
		float max_y = -1000000;
		for (int c = 0; c < SHAPE_SIDES; c++) {
			if (trianglelist[i].v[c].y < min_y) {
				min_y = trianglelist[i].v[c].y;
			}
			if (trianglelist[i].v[c].y > max_y) {
				max_y = trianglelist[i].v[c].y;
			}

		}
		trianglelist[i].shape_min_y = min_y;
		trianglelist[i].shape_max_y = max_y;

	}
	/*for (int i = 0; i < numtriangles; i++) {

		scan_convert(trianglelist[i]);
		}*/


	// Now read lights
	//cout << numlights << endl;
	lightlist = new light[numlights];
	infile >> ambientlight.r >> ambientlight.g >> ambientlight.b;

	//set_ambient_pixels(ambientlight);
	for (i = 0; i < numlights; i++) {
		infile >> lightlist[i].x >> lightlist[i].y >> lightlist[i].z;
		infile >> lightlist[i].brightness.r >> lightlist[i].brightness.g >> lightlist[i].brightness.b;
		//cout << lightlist[i] << endl;
	}


	// Now read textures
	texturelist = new texture[numtextures];
	for (i = 0; i < numtextures; i++) {
		infile >> texturelist[i].xsize >> texturelist[i].ysize;
		texturelist[i].elements = new float[texturelist[i].xsize*texturelist[i].ysize * 3];
		for (j = 0; j < texturelist[i].xsize; j++) {
			for (k = 0; k < texturelist[i].ysize; k++) {
				infile >> texturelist[i].elements[3 * (j*texturelist[i].ysize + k)];
				infile >> texturelist[i].elements[3 * (j*texturelist[i].ysize + k) + 1];
				infile >> texturelist[i].elements[3 * (j*texturelist[i].ysize + k) + 2];
			}
		}
	}

	infile.close();
	// compute the normal equations of the triagles
	for (int i = 0; i < numtriangles; i++) {
		compute_plane_equation(trianglelist[i].v, trianglelist[i]);
	}

	
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(ImageW, ImageH);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(" Sarah Sahibzada - Homework 6");

	init();

	glutDisplayFunc(display);
	glutMainLoop();
	
	return 0;
}