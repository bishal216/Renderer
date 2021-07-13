#pragma once
template <class T>
void swap(T& a, T& b) {
    auto temp = a;
    a = b;
    b = temp;
}