#include "MaterialParse.h"

MaterialParse::MaterialParse(std::string filename)
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

    std::string line,strash;

    MaterialData temp;
    
    char trash;
    while (!in.eof())
    {
        //get one line at a time
        std::getline(in, line);
        //string object
        std::istringstream iss(line.c_str());

       
        if (!line.compare(0, 6, "newmtl"))  //New Material
        {
            if(temp.MaterialNumber)
                MaterialList.push_back(temp);   //This pushes MaterialDate into vector
            iss >> strash >> strash;
             strash= (strash.substr(strash.size() - 3));
             std::stringstream tempss(strash);
             tempss >> temp.MaterialNumber;
        }
        if (!line.compare(0, 2, "Ns"))  //Shininess
        {
            iss >> trash>>trash >>temp.Ns;      // line is in the form ofNs 1.00000
        }
        if (!line.compare(0, 2, "Ka"))  //Ambient colour
        {
            iss >> trash >> trash >> temp.Ka.x>>temp.Ka.y>>temp.Ka.z;      // line is in the form of Ka 0.1 0.2 0.3
        }
        if (!line.compare(0, 2, "Kd"))  //Diffuse colour
        {
            iss >> trash >> trash >> temp.Kd.x >> temp.Kd.y >> temp.Kd.z;      // line is in the form of Kd 0.1 0.2 0.3
        }
        if (!line.compare(0, 2, "Ks"))  //Specular colour
        {
            iss >> trash >> trash >> temp.Ks.x >> temp.Ks.y >> temp.Ks.z;      // line is in the form of Kd 0.1 0.2 0.3
        }
    }
    MaterialList.push_back(temp);   //pushes last Material
    std::cout<<nMaterials();
    for (int i = 0; i < nMaterials(); i++)
        PrintMaterialList(i);
}

MaterialParse::~MaterialParse()
{
}

int MaterialParse::nMaterials()
{
    return MaterialList.size(); 
}

MaterialData MaterialParse::Material(int materialNumbeer)
{
    for(int i=0;i<nMaterials();i++)
        if(MaterialList.at(i).MaterialNumber == materialNumbeer)
            return MaterialList.at(i);
    std::cerr << "No such material";
    exit(2);
}

void MaterialParse::PrintMaterialList(int i)
{
    std::cout << std::endl;
    std::cout << "MaterialNumber: " << MaterialList.at(i).MaterialNumber << std::endl;
    std::cout << "Ambient: " << MaterialList.at(i).Ka ;
    std::cout << "Diffuse: " << MaterialList.at(i).Kd ;
    std::cout << "Specular: " << MaterialList.at(i).Ks ;
    std::cout << "Alpha: " << MaterialList.at(i).Ns << std::endl;
    std::cout << std::endl;
}
