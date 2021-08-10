#pragma once
template <class T>
void swap(T& a, T& b) {
    auto temp = a;
    a = b;
    b = temp;
}

template <class T>
T higher(T a, T b)
{
    if (a > b)
        return a;
    return b;
}

template <class T>
T lower(T a, T b)
{
    if (a < b)
        return a;
    return b;
}

template <class T>
T clamp(T val, T lower = 0, T upper = 1)
{
    if (val < lower)
        return lower;
    if (val > upper)
        return upper;
    return val;
}