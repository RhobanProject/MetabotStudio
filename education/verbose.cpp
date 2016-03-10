#include "verbose.h"

static int level = 0;

void setVerbosity(int level_)
{
    level = level_;
}

bool isVerbose(int level_)
{
    return level >= level_;
}
