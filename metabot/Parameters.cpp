#include "Parameters.h"

namespace Metabot
{

    Parameters::Parameters()
    {
    }

    void Parameters::set(std::string name, std::string value)
    {
        get(name).value = value;
    }

    Parameter &Parameters::get(std::string name)
    {
        if (!count(name)) {
            (*this)[name].name = name;
        }

        return (*this)[name];
    }
            
    std::string Parameters::toArgs()
    {
        std::string args = "";
        
        for (auto parameter : *this) {
            args += "-D" + parameter.second.name + "=" + parameter.second.value + " ";
        }

        return args;
    }
}
