#pragma once

#include <string>

#ifndef RAD2DEG
#define RAD2DEG(a) ((a)*180.0/M_PI)
#endif
#ifndef DEG2RAD
#define DEG2RAD(a) ((a)*M_PI/180.0)
#endif

std::string homeDir(std::string path);
std::string execute(std::string command);
double getTime();

int uniform(int a, int b);
double randDouble();

template<typename T>
T &randVec(std::vector<T> &vec)
{
    auto k = uniform(0, vec.size()-1);
    return vec[k];
}
