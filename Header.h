
#pragma once
#ifndef initialization
#define initialization

//#include<iostream>
#include<GL/glut.h>
//#include"maths.h"
#include"ModelParse.h"
#include"Transformations.h"

#define WIDTH 800
#define HEIGHT 800
#define FPS 60
#define Pi 3.14159
//functions for drawing
void initcanvas(int argc, char** argv);		//Initialize GLUT and canvas
void reshape(int w, int h);					//Resizing of window
void update(int);							//update
void display();								//display
void cleargrid();							//Clears grid
void myKeyboardFunc(unsigned char key, int x, int y);	//Keyboard 
void myMouseFunc(int button, int state, int x, int y);	//mouse
void myPassiveMotionFunc(int x, int y); //motion when not clicked

														//putpixel
void putpixel(vec3 P, const vec3& col);

//Our functions
void LineBresenham_adjusted(vec3 v1, vec3 v2, const vec3_T<float>& color);
void Flatrasterize(vec3 V1, vec3 V2, vec3 V3, const vec3_T<float>& color);
void triangle(vec3* pts, float* intensity, vec3 ka, vec3 kd);
void triangle(vec3* world,vec3* pts, vec3* normal, vec3 ka, vec3 kd, vec3 ks,float alpha);
//For Camera
vec3 world2screen(vec3 v);
vec3 transform(vec3 pts);

//variables
bool* grid;
vec3* color;
float* zBuffer;


//ModelParse* object = new ModelParse("Object/Stupa4.obj");
//ModelParse* object = new ModelParse("Object/chopper.obj");
//ModelParse* object = new ModelParse("Object/swyambutemples.obj");
//ModelParse* object = new ModelParse("Object/untitled.obj");
ModelParse* object = new ModelParse("Object/finalObj2.obj");


int width = WIDTH;
int height = HEIGHT;
int depth = 1500;
bool doPers = true;
bool lightRevolve = false;
bool cull = false;
int theta = 0,stepsize=1;
//vec3 camera = { 0,0,3 };	
vec3 light_dir = vec3(1000, 0, 0).normalize();
vec3 eye = { 0,0,800 };
vec3 translate = vec3(0, 0, 0), rotate = vec3(0, 0, 0), scale = vec3(1, 1, 1);
vec3 lookAt = { 1.0f *width / 2,1.f*height / 2,0 };
float rad = (lookAt - eye).getmag();
//colors
const vec3_T<float>& WHITE = { 1,1,1 };
const vec3_T<float>& RED = { 1,0,0 };
const vec3_T<float>& BLUE = { 0,1,0 };
const vec3_T<float>& GREEN = { 0,0,1 };
/*
std::vector<std::vector<vec3i>> faceList;	//FaceList
std::vector<vec3i> face;					//Individual Face struct
*/
std::vector<FaceData> fDList;
FaceData fD;
vec3 points[3], world[3],n[3];	//Screen and World co-ords
float intensity[3];         //intensity values for Gouraud shading

std::vector<vec3> vertexList;	//list of vertices
vec3 v;						//vertex data from face

vec3 normal;				//normal data from face
vec3 surfaceNormal;			//normal of the surface;Obtained from cross product
vec3 CoG;					//cross product for centre og gravity
vec2 bboxmin, bboxmax;		//gets boundry box
//------------------EXPERIMENTAL------------------------
void triangle(vec3* pts, float* zbuffer, const vec3_T<float>& color, float* intensity);
enum rastermode{vertexGrid,wireframe,flat,gauraud,phong};
rastermode rMode = wireframe;
void Drawface(FaceData face);
void Rotatelight();
#endif