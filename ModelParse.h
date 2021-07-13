#pragma once
#ifndef __MODEL_H__
#define __MODEL_H__

#include "maths.h"
#include<vector>
class ModelParse 
{
private:
	std::vector<vec3> vertices;
	std::vector<std::vector<int>> faces;
public:
	ModelParse(std::string filename);
	~ModelParse();
	int nverts();
	int nfaces();
	vec3 vert(int i);
	std::vector<int> face(int idx);
};

#endif

