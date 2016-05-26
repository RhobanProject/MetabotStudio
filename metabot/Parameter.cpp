#include "Parameter.h"

namespace Metabot
{
    Parameter::Parameter()
        : expose(false)
    {
    }

    Parameter::Parameter(std::string name_, std::string value_, std::string description_)
        : name(name_), value(value_), description(description_), expose(false)
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

    double Parameter::getNumericValue()
    {
        return atof(value.c_str());
    }

    double Parameter::getMin()
    {
        return atof(min.c_str());
    }

    double Parameter::getMax()
    {
        return atof(max.c_str());
    }
}
