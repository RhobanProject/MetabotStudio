#include <iostream>
#include "Part.h"

namespace Metabot
{
    Part::Part(std::string name_, std::string params_)
        : name(name_), params(params_)
    {
        // std::cout << "Part: " << name << ", " << params << std::endl;
    }
}
