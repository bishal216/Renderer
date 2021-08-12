#pragma once
#include"maths.h"
template<class T>
class transformation3D
{
public:
    static vec3_T<T> scale(vec3_T<T> &point, vec3_T<T> &scaleFactor)
    {
        vec3_T<T> temp;
        temp.x = point.x * scaleFactor.x;
        temp.y = point.y * scaleFactor.y;
        temp.z = point.z * scaleFactor.z;
        return temp;
    }
    static vec3_T<T> scalePoint(vec3_T<T> point, vec3_T<T> scaleFactor, vec3_T<T> centre) {
        point =translate(point, -centre);
        point =scale(point, scaleFactor);
        point =translate(point, centre);
        return point;
    }
    static vec3_T<T> translate(vec3_T<T> &point, vec3_T<T> &translationVector)
    {
        vec3_T<T> temp;
        temp.x = point.x + translationVector.x;
        temp.y = point.y + translationVector.y;
        temp.z = point.z + translationVector.z;
        return temp;
    }

    static vec3_T<T> rotate(vec3_T<T> &point, vec3_T<T>& angle)
    {
        vec3_T<T> temp = point;

        //X-rotation
        if ((angle.x) != 0)
        {
            temp.y = (1.0f * point.y * cos(angle.x) - 1.0f * point.z * sin(angle.x));
            temp.z = (1.0f * point.y * sin(angle.x) + 1.0f * point.z * cos(angle.x));
            point = temp;
        }
        //Y-rotation
        if (angle.y != 0)
        {
            temp.x = (1.0f * point.z * sin(angle.y) + 1.0f * point.x * cos(angle.y));
            temp.z = (1.0f * point.z * cos(angle.y) - 1.0f * point.x * sin(angle.y));
            point = temp;
        }
        //Z-rotation
        if (angle.z != 0)
        {
            temp.x = (-1.0f * point.y * sin(angle.z) + 1.0f * point.x * cos(angle.z));
            temp.y = (1.0f * point.y * cos(angle.z) + 1.0f * point.x * sin(angle.z));
            point = temp;
        }
        return temp;
    }
   
    static vec3_T<T> reflect(vec3_T<T>& point, bool* ifReflect, vec3_T<T>& axis)
    {
        vec3_T<T> temp = point;
        if (ifReflect[0] == 1)
            temp.x = 2 * axis.x - point.x;
        if (ifReflect[1] == 1)
            temp.y = 2 * axis.y - point.y;
        if (ifReflect[2] == 1)
            temp.z = 2 * axis.z - point.z;
        return temp;
    }
    static vec3_T<T> shear(vec3_T<T>& point, vec3_T<T>& shearFactor)
    {
        vec3_T<T> temp = point;
        //Shear along X
        temp.y = point.y + shearFactor.y * point.z;
        temp.z = point.z + shearFactor.z * point.y;
        point = temp;
        //Shear along Y
        temp.x = point.x + shearFactor.x * point.z;
        temp.z = point.z + shearFactor.z * point.x;
        point = temp;
        //Shear along Z
        temp.x = point.x + shearFactor.x * point.y;
        temp.y = point.y + shearFactor.y * point.x;

        return temp;
    }
};
