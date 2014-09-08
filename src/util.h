#pragma once

#include <vector>
#include <algorithm>
#include <string>

std::vector<std::string> split(const std::string &s, char delim);
std::string implode(std::vector<std::string> strs, std::string separator);
std::string basename(std::string const pathname);
std::string ltrim(std::string s);
std::string rtrim(std::string s);
std::string trim(std::string s);
std::string tempname();
bool file_exists(std::string filename);
void file_put_contents(std::string path, std::string contents);
std::string file_get_contents(std::string path);
bool startswith(std::string str, std::string start);
std::string removestart(std::string str, std::string start);
