
#pragma once
#ifndef initialization
#define initialization

//#include<iostream>
#include<GL/glut.h>
//#include"maths.h"
#include"ModelParse.h"
#include"Transformations.h"

#define WIDTH 500
#define HEIGHT 500
#define FPS 60

//functions for drawing
void initcanvas(int argc, char** argv);		//Initialize GLUT and canvas
void reshape(int w, int h);					//Resizing of window
void update(int);							//update
void display();								//display
void cleargrid();							//Clears grid
void myKeyboardFunc(unsigned char key, int x, int y);	//Keyboard 

														//putpixel
void putpixel(int x, int y, float zBuf, const vec3& col = 1,bool flat = false);

//Our functions
void LineBresenham_adjusted(int x1, int y1, int x2, int y2, const vec3_T<float>& color);
void Triangle(vec2i t0, vec2i t1, vec2i t2, const vec3_T<float>& color, bool wireframe = true);
void rasterize(vec2i t0, vec2i t1, vec2i t2, const vec3_T<float>& color);


//For Camera
vec3 world2screen(vec3 v);
vec3 transform(vec3 pts);

//variables
bool* grid;
vec3* color;
float* zBuffer;

//ModelParse* object = new ModelParse("Object/TestCube.obj");
//ModelParse* object = new ModelParse("Object/Head.obj");
//ModelParse* object = new ModelParse("Object/Stupa3.obj");
ModelParse* object = new ModelParse("Object/Stupa4.obj");
//ModelParse* object = new ModelParse("Object/WallBedChair.obj");
//ModelParse* object = new ModelParse("Object/plane.obj");


int width = WIDTH;
int height = HEIGHT;
int depth = 500;
bool doPers = true;

//vec3 camera = { 0,0,3 };	
vec3 light_dir = vec3(0, 1, 1).normalize();
vec3 eye = { 0,0,3 };
vec3 translate = vec3(0, 0, 0), rotate = vec3(0, 0, 0), scale = vec3(1, 1, 1);

//colors
const vec3_T<float>& WHITE = { 1,1,1 };
const vec3_T<float>& RED = { 1,0,0 };
const vec3_T<float>& BLUE = { 0,1,0 };
const vec3_T<float>& GREEN = { 0,0,1 };

std::vector<std::vector<vec3i>> faceList;	//FaceList
std::vector<vec3i> face;					//Individual Face struct

vec3 points[3], world[3];	//Screen and World co-ords
float intensity[3];         //intensity values for Gouraud shading

std::vector<vec3> vertexList;	//list of vertices
vec3 v;						//vertex data from face

vec3 normal;				//normal data from face
vec3 CoG;					//cross product for centre og gravity
vec2 bboxmin, bboxmax;		//gets boundry box
//------------------EXPERIMENTAL------------------------
void triangle(vec3* pts, float* zbuffer, const vec3_T<float>& color, float* intensity);
#endif