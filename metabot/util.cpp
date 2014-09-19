#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <fcntl.h>
#include <dirent.h>
#include "util.h"
#include "sha1.h"
#include <sys/stat.h>
#include <sys/types.h>

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> parts;

    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim)) {
        parts.push_back(item);
    }
    return parts;
}

std::string implode(std::vector<std::string> strs, std::string separator)
{
    std::stringstream str;
    for (unsigned int i=0; i<strs.size(); i++) {
        str << strs[i];
        if (i != strs.size()-1) {
            str << separator;
        }
    }
    return str.str();
}

std::string basename(std::string const pathname)
{
    std::string buffer = "";
    int i = pathname.length()-1;

    while (i>0 && pathname[i]!='/') {
        if (pathname[i] == '.') {
            buffer = "";
        } else {
            buffer = pathname[i] + buffer;
        }
        i--;
    }
    return buffer;
}

std::string ltrim(std::string s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

std::string rtrim(std::string s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

std::string trim(std::string s) {
    return ltrim(rtrim(s));
}

std::string tempname() {
    return tempnam(NULL, "metabot");
}

bool file_exists(std::string filename)
{
    int fd = open(filename.c_str(), O_RDONLY);
    if (fd > 0) {
        close(fd);
        return true;
    } else {
        return false;
    }
}

void file_put_contents(std::string path, std::string contents)
{
    std::ofstream ofile(path.c_str());

    if (ofile) {
        ofile << contents;
        ofile.close();
    }
}

std::string file_get_contents(std::string path)
{
    std::ifstream ifs(path.c_str());
    std::string content((std::istreambuf_iterator<char>(ifs)),
            (std::istreambuf_iterator<char>()));
    return content;
}

bool startswith(std::string str, std::string start)
{
    return str.substr(0, start.length())==start;
}

std::string removestart(std::string str, std::string start)
{
    return str.substr(start.length());
}

std::string hash_sha1(std::string str)
{
    unsigned char hash[20];
    char hexstring[41];
    sha1::calc(str.c_str(), str.length(), hash); // 10 is the length of the string
    sha1::toHexString(hash, hexstring);

    return std::string(hexstring, 41);
}

bool is_directory(std::string path)
{
    DIR *dir;
    dir = opendir(path.c_str());
    if (dir != NULL) {
        closedir(dir);
        return true;
    } else {
        return false;
    }
}

void makedir(std::string path)
{
    mkdir(path.c_str(), 0755);
}

std::vector<std::string> get_directory_listing(std::string path)
{
    std::vector<std::string> listing;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string entry = std::string(ent->d_name);
            if (entry != "." && entry != "..") {
                listing.push_back(entry);
            }
        }
        closedir(dir);
    }

    return listing;
}
