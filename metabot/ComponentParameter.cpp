#include "ComponentParameter.h"

namespace Metabot
{
    ComponentParameter::ComponentParameter(std::string name_, std::string value_, std::string description_)
        : name(name_), value(value_), description(description_)
    {
    }

    ComponentParameter::~ComponentParameter()
    {
    }

    bool ComponentParameter::isBool()
    {
        return (value=="true" || value == "false");
    }
}
