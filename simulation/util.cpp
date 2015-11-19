#include <chrono>
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
