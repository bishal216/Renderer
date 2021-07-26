#pragma once
#ifndef __MODEL_H__
#define __MODEL_H__

#include "maths.h"
#include<vector>
class ModelParse 
{
private:
	std::vector<vec3> vertices;				//vertex array
	std::vector<std::vector<vec3i>> faces;	//face array
	std::vector<vec2> textures;	//texture co-ordinates array
	std::vector<vec3> normal;	//normal co-ordinates array for Gouraud Shading
public:
	ModelParse(std::string filename);
	~ModelParse();
	int nverts();	//returns number of vertices
	int nfaces();	//returns number of faces
	int ntextures();	//returns number of texture vertices
	int nnormal();		//returns number of normal vertices

	vec3 vert(int i);	//returns i-th vertices
	std::vector<vec3i> face(int idx);	//returns idx-th face
	vec3 norm(int idx, int i);		//returns normal of idx-th face's i-th vertex 
};

#endif

