#pragma once

#include <map>
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
std::vector<std::string> split(const std::string &s, char delim=' ', int limit=-1);
std::vector<std::string> splitCSV(const std::string s, char delim=' ', char enclosure='"');
std::map<std::string, std::string> getVars(const std::string s, char delim=',', char enclosure='"', char equals='=');
std::string implode(std::vector<std::string> strs, std::string separator);
std::string ltrim(std::string s);
std::string rtrim(std::string s);
std::string trim(std::string s);
bool startswith(std::string str, std::string start);
bool endswith(std::string str, std::string end);
std::string removestart(std::string str, std::string start);
std::string hash_sha1(std::string str);
std::string replaceStr(std::string& str, const std::string& from, const std::string& to);
std::string strtolower(std::string input);

// Path manipulation
std::string basename(std::string const pathname);
std::string tempname();
bool file_exists(std::string filename);
bool is_directory(std::string path);
std::vector<std::string> get_directory_listing(std::string path);
void makedir(std::string path);
void file_put_contents(std::string path, std::string contents);
std::string file_get_contents(std::string path);
int filemtime(std::string path);
void remove_file(std::string filename);
std::string current_dir();

// Random
int random_int();
double random_number(double min, double max, double precision=0.0001);

static inline double bound(double x, double min, double max)
{
    if (x < min) return min;
    if (x > max) return max;
    return x;
}
