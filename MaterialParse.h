#pragma once
#include"maths.h"
#include<vector>
#include<fstream>
#include<sstream>
#include<string>
#include<strstream>

struct MaterialData {
	int MaterialNumber =0;	//newmtl Material.001
	float Ns = 0;	//shininess (alpha)
	vec3 Ka = 0;	//ambient colour
	vec3 Kd = 0;	//diffuse colour
	vec3 Ks = 0;	//specular colour

	//mtl also has these but we don't use them
	//vec3 Ke;	//Transmission Filter 
	//vec3 Ni;	//Refraction coefficient
	// float d;//opacity, represented by d in mtl, we don't work with transparency
	// int illum;	//type of illumination
};
class MaterialParse
{
	private:
		std::vector<MaterialData> MaterialList;
	public:
		MaterialParse(std::string filename);
		~MaterialParse();
		int nMaterials();
		MaterialData Material(int materialNumbeer);
		void PrintMaterialList(int i);
};

