#pragma once
#ifndef initialization
#define initialization
//#include<iostream>
#include<GL/glut.h>
//#include"maths.h"
#include"ModelParse.h"
#define WIDTH 800
#define HEIGHT 800
#define FPS 60
//functions for drawing
void initcanvas(int argc, char** argv);		//Initialize GLUT and canvas
void reshape(int w, int h);					//Resizing of window
void update(int);							//update
void display();								//display
void cleargrid();							//Clears grid

//putpixel
void putpixel(int x, int y,float zBuf, const vec3& col = 1);
void putpixel_adjusted(int x, int y,float zBuf, const vec3_T<float>& col = 0);

//Our functions
void LineBresenham_adjusted(int x1, int y1, int x2, int y2, const vec3_T<float>& color);
void Triangle(vec2i t0, vec2i t1, vec2i t2, const vec3_T<float>& color, bool wireframe = true);
void rasterize(vec2i t0, vec2i t1, vec2i t2, const vec3_T<float>& color);

//For Camera
mat4 lookat(vec3 eye, vec3 center, vec3 up);
mat4 viewport(int x, int y, int w, int h);
vec3 world2screen(vec3 v);

//variables
bool* grid;
vec3* color;
float* zBuffer;
//ModelParse* object = new ModelParse("Object/TestCube.obj");
//ModelParse* object = new ModelParse("Object/Head.obj");
ModelParse* object = new ModelParse("Object/Stupa3.obj");
int width = WIDTH;
int height = HEIGHT;
int depth = 500;


//vec3 camera = { 0,0,3 };
vec3 light_dir = vec3( 0,1,1 ).normalize();;
vec3 eye = { 0,0,3 };
vec3 center = { 0,0,0 };
vec3 up = { 0,0,1 };

//colors
const vec3_T<float>& WHITE = { 1,1,1 };
const vec3_T<float>& RED = { 1,0,0 };
const vec3_T<float>& BLUE = { 0,1,0 };
const vec3_T<float>& GREEN = { 0,0,1 };

float scale =0.05;
//structs
struct clr { float R; float G; float B; float A; };


//------------------EXPERIMENTAL------------------------
void triangle(vec3* pts, float* zbuffer, const vec3_T<float>& color, float* intensity);
vec3 rotation3D(vec3 Old, float angleX, float angleY, float angleZ);
void spin(bool X,bool Y, bool Z);
#endif