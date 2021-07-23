#pragma once
#ifndef __MODEL_H__
#define __MODEL_H__

#include "maths.h"
#include<vector>
class ModelParse 
{
private:
	std::vector<vec3> vertices;				//vertex array
	std::vector<std::vector<int>> faces;	//face array
	std::vector<vec2> textures;	//texture co-ordinates array
public:
	ModelParse(std::string filename);
	~ModelParse();
	int nverts();	//returns number of vertices
	int nfaces();	//returns number of faces
	int ntextures();	//returns number of texture vertices
	vec3 vert(int i);
	std::vector<int> face(int idx);
};

#endif

