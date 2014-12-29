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
        if (!parameters.count(name)) {
            parameters[name].name = name;
        }

        return parameters[name];
    }

    std::map<std::string, Parameter> &Parameters::getAll()
    {
        return parameters;
    }
            
    std::string Parameters::toArgs()
    {
        std::string args = "";
        
        for (auto parameter : parameters) {
            args += "-D" + parameter.second.name + "=" + parameter.second.value + " ";
        }

        return args;
    }
}
