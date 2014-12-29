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

    bool Parameter::isBool()
    {
        return (value=="true" || value == "false");
    }
}
