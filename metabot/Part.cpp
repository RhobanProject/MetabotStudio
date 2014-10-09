#include <iostream>
#include "Part.h"
#include "util.h"

namespace Metabot
{
    Part::Part(std::string name_, std::string params_)
        : name(name_), params(params_), quantity(1)
    {
        // std::cout << "Part: " << name << ", " << params << std::endl;
    }
            
    Part *Part::clone()
    {
        return new Part(name, params);
    }
            
    std::string Part::hash()
    {
        return hash_sha1(name+"//"+params);
    }
}
