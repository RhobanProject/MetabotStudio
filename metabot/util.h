#pragma once

#include <vector>
#include <algorithm>
#include <string>

#ifndef DEG2RAD
#define DEG2RAD(x) (((x)/180.0)*M_PI)
#endif
#ifndef RAD2DEG
#define RAD2DEG(x) (((x)/M_PI)*180.0)
#endif

// String manipuliation
std::vector<std::string> split(const std::string &s, char delim);
std::string implode(std::vector<std::string> strs, std::string separator);
std::string ltrim(std::string s);
std::string rtrim(std::string s);
std::string trim(std::string s);
bool startswith(std::string str, std::string start);
std::string removestart(std::string str, std::string start);
std::string hash_sha1(std::string str);

// Path manipulation
std::string basename(std::string const pathname);
std::string tempname();
bool file_exists(std::string filename);
bool is_directory(std::string path);
std::vector<std::string> get_directory_listing(std::string path);
void makedir(std::string path);
void file_put_contents(std::string path, std::string contents);
std::string file_get_contents(std::string path);
