#pragma once
#ifndef initialization
#define initialization
//#include<iostream>
#include<GL/glut.h>
#include<math.h>
#include"Transformations.h"
//#include"maths.h"
#include"ModelParse.h"
#include"GenericFunctions.h"
#include<chrono>
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

//Our functions
//---------------Projection----------------------------
vec3 reverseNormalization(vec3 point);
vec3 project(vec3 point);
//------------------Raster------------------------
void triangle(vec3* pts,  const vec3_T<float>& color, vec3* L, vec3* N, vec3* V, vec3* R,float* intensity,bool mode);//Normal interpolation


//variables
bool* grid;
vec3* color;
float* zBuffer;
//ModelParse* object = new ModelParse("Object/TestCube.obj");
//ModelParse* object = new ModelParse("Object/Head.obj");
//ModelParse* object = new ModelParse("Object/Stupa3.obj");
ModelParse* object = new ModelParse("Object/untitled.obj");
int width = 800;
int height = 800;
int zMax = 800;
vec3 light_dir = vec3(-1000,0,0);
vec3 prp = vec3( 0,0,2500);

//Phong
float ka = 0.2, kd = 0.4 ,ks = 1;
float Ia = 1, Il = 1;
float alpha = 200;
#endif