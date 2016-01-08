#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"

using namespace std::chrono;

std::string homeDir(std::string path)
{
    std::string prefix = "";
    char *host = getenv("HOME");

    if (host != NULL) {
        prefix = std::string(host);
    }

    return prefix + "/" + path;
}

double getTime()
{
    auto tp = steady_clock::now();
    return duration_cast<std::chrono::nanoseconds>(tp.time_since_epoch()).count() / 1000000000.0;
}

std::string execute(std::string command)
{
    std::string result;
    FILE *process = popen(command.c_str(), "r");
    int c;
    while ((c = fgetc(process)) > 0) {
        result += c;
    }   
    pclose(process);
    return result;
}
