#include "ModelParse.h"
#include<fstream>
#include<sstream>
#include<string>
ModelParse::ModelParse(std::string filename) : vertices(), faces()
{
    std::cout << "Filename : " << filename;
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
            vertices.push_back(v);
        }
        else if (!line.compare(0, 2, "f ")) //starts with f<space>
        {
            std::vector<int> f;
            int itrash, idx;
            iss >> trash; //first charecter is f
            while (iss >> idx >> trash >> itrash >> trash >> itrash)  // in the form vert/vertTex/norm (vert is read, the rest are treated as trash)
            {
                idx--; // in wavefront obj all indices start at 1, not zero
                f.push_back(idx);
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
    }
    std::cout << nverts() << "," << nfaces() << "," << ntextures();
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

vec3 ModelParse::vert(int i)
{
    return vertices[i];
}

std::vector<int> ModelParse::face(int idx)
{
    return faces[idx];
}