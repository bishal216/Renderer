#include "ModelParse.h"


ModelParse::ModelParse(std::string filename) : vertices()//, faces()
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
    char trash;
    std::string str;
    int MaterialNumber;
    while (!in.eof())
    {
        //get one line at a time
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        if (!line.compare(0, 6, "mtllib"))  
        {
            iss >> str >> str;
            materialPath = "Object/" + str;

        }
    }
    in.clear();
    in.seekg(0);
    MaterialParse MaterialObject(materialPath);
    while (!in.eof())
    {
        //get one line at a time
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        
        if (!line.compare(0, 6, "usemtl"))  //New Material
        {
            iss >> str >> str;
            str = (str.substr(str.size() - 3));
            std::stringstream tempss(str);
            tempss >> MaterialNumber;
        }
        else if (!line.compare(0, 2, "v "))  //starts with v<space>
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
            FaceData fD;
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
            //faces.push_back(f);
            for (int i = 0; i < 3; i++)
            {
                fD.vertices[i] = vert(f.at(i).x);
                fD.normal[i] = norm(f.at(i).z);
            }
            fD.mtl = MaterialObject.Material(MaterialNumber);
            FaceDataList.push_back(fD);
            /*
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
            MaterialNumberList.push_back(MaterialNumber);
            */
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
    //std::cout << nfaceData()<<std::endl;
    //manageFaces();
}

ModelParse::~ModelParse()
{
}

int ModelParse::nverts()
{
    return (int)vertices.size();
}
/*
int ModelParse::nfaces()
{
    return (int)faces.size();
}
*/
int ModelParse::nfaceData()
{
    return FaceDataList.size();
}


int ModelParse::nnormal()
{
    return(int)normal.size();
}

vec3 ModelParse::vert(int i)
{
    return vertices.at(i);
}
/*
std::vector<vec3i> ModelParse::face(int idx)
{
    return faces.at(idx);
}
*/
FaceData ModelParse::returnFaceData(int idx)
{
    return FaceDataList.at(idx);
}

vec3 ModelParse::norm(int i)
{
    return normal.at(i);
}
/*
vec3 ModelParse::norm(int idx, int i)
{
    return normal.at(faces.at(idx).at(i).z).normalize();
}
*/
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

void ModelParse::manageFaces()
{
    vec3 range = maxValue - minValue;
    vec3 offSet = range / 2 - maxValue;
    //std::cout << std::endl << range << std::endl << offSet;
    for (int i = 0; i < FaceDataList.size(); i++)
    {
        for (int j = 0; j < 3; j++)
        {
            FaceDataList.at(i).vertices[j] += offSet;
            FaceDataList.at(i).vertices[j] /= (range / 2);
            FaceDataList.at(i).vertices[j] += 1.f;
        }
        
    }
}
