#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <random>
#include <fcntl.h>
#include <dirent.h>
#include "util.h"
#include "sha1.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

std::vector<std::string> split(const std::string &s, char delim, int limit) {
    if (limit > 0) limit--;
    std::vector<std::string> parts;

    std::stringstream ss(s);
    std::string item;
    while (limit!=0 && getline(ss, item, delim)) {
        parts.push_back(item);
        limit--;
    }
    unsigned int c;
    std::string end = "";
    while ((c = ss.get()) <= 256) {
        end += (char)c;
    }
    if (end != "") {
        parts.push_back(end);
    }
    return parts;
}

std::vector<std::string> splitCSV(const std::string s, char delim, char enclosure)
{
    std::vector<std::string> parts;
    std::string buffer = "";
    bool in_enclosure = false;

    for (unsigned int i=0; i<s.size(); i++) {
        char c = s[i];

        // XXX: This implementation is little bit naive
        if (c == enclosure) {
            in_enclosure = !in_enclosure;
        } else {
            if (in_enclosure) {
                buffer += c;
            } else {
                if (c == delim) {
                    if (buffer != "") {
                        parts.push_back(buffer);
                        buffer = "";
                    }
                } else {
                    buffer += c;
                }
            }
        }
    }
    if (buffer != "") {
        parts.push_back(buffer);
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

bool endswith(std::string str, std::string end)
{
    return str.substr(str.length()-end.length(), end.length())==end;
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

std::string replaceStr(std::string& str, const std::string& from, const std::string& to) {
    std::string output = str;
    size_t start_pos = output.find(from);
    if (start_pos != std::string::npos) {
        output.replace(start_pos, from.length(), to);
    }
    return output;
}

std::string strtolower(std::string input)
{
    for (unsigned int i=0; i<input.length(); i++) {
        input[i] = tolower(input[i]);
    }
    return input;
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
#ifdef WIN32
    mkdir(path.c_str());
#else
    mkdir(path.c_str(), 0755);
#endif
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

int filemtime(std::string path)
{
    struct stat fstat;
    if (stat(path.c_str(), &fstat) == 0) {
        return fstat.st_mtime;
    }

    return 0;
}

void remove_file(std::string filename)
{
    unlink(filename.c_str());
}

std::string current_dir()
{
    return std::string(get_current_dir_name());
}

class Random
{
    public:
        Random() {
            m.seed(time(NULL));
        };
        unsigned int random() {
            return m();
        };
    protected:
        std::mt19937 m;
};

static Random random_generator;

int random_int()
{
    return random_generator.random();
}

double random_number(double min, double max, double precision)
{
    unsigned int delta = (max-min)/precision;
    return ((random_int()%delta)*precision)+min;
}
