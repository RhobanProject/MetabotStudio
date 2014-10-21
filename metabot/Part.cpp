#include <iostream>
#include "Part.h"
#include "util.h"

namespace Metabot
{
    Part::Part(std::string name_, std::string params_, TransformMatrix matrix_)
        : name(name_), params(params_), quantity(1), matrix(matrix_)
    {
        // std::cout << "Part: " << name << ", " << params << std::endl;
    }
            
    Part *Part::clone()
    {
        return new Part(name, params, matrix);
    }
            
    std::string Part::hash()
    {
        return hash_sha1(name+"//"+params);
    }
}
