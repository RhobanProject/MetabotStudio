#include <stdlib.h>
#include "util.h"

std::string homeDir(std::string path)
{
    std::string prefix = "";
    char *host = getenv("HOME");

    if (host != NULL) {
        prefix = std::string(host);
    }

    return prefix + "/" + path;
}
