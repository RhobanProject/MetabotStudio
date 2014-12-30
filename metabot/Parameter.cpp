#include "Parameter.h"

namespace Metabot
{
    Parameter::Parameter()
    {
    }

    Parameter::Parameter(std::string name_, std::string value_, std::string description_)
        : name(name_), value(value_), description(description_)
    {
    }

    Parameter::~Parameter()
    {
    }

    void Parameter::updateValue(std::string newValue)
    {
        if (isBool()) {
            value = (newValue == "true") ? "true" : "false";
        } else if (isString()) {
            value = "\"" + newValue + "\"";
        } else {
            value = newValue;
        }
    }

    bool Parameter::isBool()
    {
        return (value=="true" || value == "false");
    }

    bool Parameter::isString()
    {
        return value.size() && value[0] == '"';
    }
            
    std::string Parameter::getValue()
    {
        if (isString()) {
            return value.substr(1, value.length()-1);
        } else {
            return value;
        }
    }
}
