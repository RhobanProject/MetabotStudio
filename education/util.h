#pragma once

#include <string>

std::string homeDir(std::string path);
std::string execute(std::string command);
double getTime();

int uniform(int a, int b);

template<typename T>
T &randVec(std::vector<T> &vec)
{
    auto k = uniform(0, vec.size()-1);
    return vec[k];
}
