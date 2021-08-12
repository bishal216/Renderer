#pragma once
#ifndef __MODEL_H__
#define __MODEL_H__

#include "MaterialParse.h"

struct FaceData {
	vec3 vertices[3];
	vec3 normal[3];
	MaterialData mtl;
	//obj also has this but we don't use it
	//vec3 textureUV[3]
};





class ModelParse 
{
private:
	std::string materialPath;

	std::vector<vec3> vertices;				//vertex array
	//std::vector<std::vector<vec3i>> faces;	//face array
	//std::vector<int> MaterialNumberList;		//Materials Number Array
	std::vector<vec3> normal;	//normal co-ordinates array for Gouraud Shading
	
	std::vector<FaceData> FaceDataList;
	//for maintaining 
	vec3 maxValue = { INT_MIN,INT_MIN,INT_MIN }, minValue = { INT_MAX,INT_MAX,INT_MAX };

public:
	ModelParse(std::string filename);
	~ModelParse();
	
	int nverts();	//returns number of vertices
	int nfaces();	//returns number of faces
	int nfaceData();	//returns number of faces
	int nnormal();		//returns number of normal vertices

	vec3 vert(int i);	//returns i-th vertices
	std::vector<vec3i> face(int idx);	//returns idx-th face
	FaceData returnFaceData(int idx);	//returns idx-th face
	vec3 norm(int i);		//returns i-th normal 
	vec3 norm(int idx, int i);		//returns normal of idx-th face's i-th vertex 


	void checkMaxMin(char mk,float vk);		//compares given value with maxk or mink with valuek
	void manageFaces(float k);				//normalizes the value between -k and k

};

#endif

