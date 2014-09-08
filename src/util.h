#pragma once

#include <vector>
#include <algorithm>
#include <string>

std::vector<std::string> split(const std::string &s, char delim);
std::string basename(std::string const pathname);
std::string ltrim(std::string s);
std::string rtrim(std::string s);
std::string trim(std::string s);
