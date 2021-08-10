#include "ModelParse.h"
#include<fstream>
#include<sstream>
#include<string>
#include<strstream>


ModelParse::ModelParse(std::string filename) : vertices(), faces()
{
    
    //std::cout << "Filename : " << filename;
    //Reads in file
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    //if file cannot be opened
    if (in.fail())
    {
        std::cout << "Cannot Read" << std::endl;
        return;
    }
    std::string line;

    while (!in.eof())
    {
        //get one line at a time
        std::getline(in, line);
        //string object
        std::istringstream iss(line.c_str());

        char trash;
        if (!line.compare(0, 2, "v "))  //starts with v<space>
        {
            iss >> trash; // first character is v
            vec3 v;
            // followed by xyz co-ords
            iss >> v.x;
            iss >> v.y;
            iss >> v.z;
            checkMaxMin('x',v.x);
            checkMaxMin('y',v.y);
            checkMaxMin('z',v.z);
            vertices.push_back(v);
        }
        else if (!line.compare(0, 2, "f ")) //starts with f<space>
        {
            std::vector<vec3i> f;
            vec3i temp;
            iss >> trash; //first charecter is f
            while (iss >> temp.x >> trash >> temp.y >> trash >> temp.z)  // in the form vert/vertTex/norm (vert is read, the rest are treated as trash)
            {
                //in wavefront obj all indices start at 1, not zero
                temp.x--;   //vert
                temp.y--;   //texture
                temp.z--; // normal 
                f.push_back(temp);
            }
            faces.push_back(f);
        }
        else if (!line.compare(0, 3, "vt "))    //starts with vt<space>
        {
            iss >> trash >> trash;//Ignore vt
            vec2 uv;
            iss >> uv.x;
            iss >> uv.y;
            textures.push_back(uv);
        }

        else if (!line.compare(0, 3, "vn "))    //starts with vn<space>
        {
            iss >> trash >> trash;//Ignore vn
            vec3 n;
            iss >> n.x;
            iss >> n.y;
            iss >> n.z;
            normal.push_back(n);
        }
       
    }
    std::cout << nverts()<<std::endl;
    manageFaces(0.8);
    
}

ModelParse::~ModelParse()
{
}

int ModelParse::nverts()
{
    return (int)vertices.size();
}

int ModelParse::nfaces()
{
    return (int)faces.size();
}

int ModelParse::ntextures()
{
    return(int)textures.size();
}

int ModelParse::nnormal()
{
    return(int)normal.size();
}

vec3 ModelParse::vert(int i)
{
    return vertices.at(i);
}

std::vector<vec3i> ModelParse::face(int idx)
{
    return faces.at(idx);
}

vec3 ModelParse::norm(int idx, int i)
{
    int temp = faces.at(idx).at(i).z;   //z is normal
    return normal.at(temp).normalize();
}

std::vector<vec3> ModelParse::returnVertices()
{
    return vertices;
}

std::vector<std::vector<vec3i>> ModelParse::returnFaces()
{
    return faces;
}

std::vector<vec3> ModelParse::returnNormals()
{
    return normal;
}

void ModelParse::checkMaxMin(char mk,float vk)
{
    switch (mk)
    {
    case 'x':
        if (vk > maxValue.x)
            maxValue.x = vk;
        if (vk < minValue.x)
            minValue.x = vk;
        break;
    case 'y':
        if (vk > maxValue.y)
            maxValue.y = vk;
        if (vk < minValue.y)
            minValue.y = vk;
        break;
    case 'z':
        if (vk > maxValue.z)
            maxValue.z = vk;
        if (vk < minValue.z)
            minValue.z = vk;
        break;
    }
}

void ModelParse::manageFaces(float k)
{
    vec3 range = maxValue - minValue;
    vec3 offSet = range / 2 - maxValue;
    //std::cout << std::endl << range << std::endl << offSet;
    for (int i = 0; i < nverts(); i++)
    {
        vertices.at(i) += offSet;
        vertices.at(i) /= (range/2);
        vertices.at(i) *= k;
    }
}
