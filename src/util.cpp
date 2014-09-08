#include <sstream>
#include "util.h"

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> parts;

    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim)) {
        parts.push_back(item);
    }
    return parts;
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
