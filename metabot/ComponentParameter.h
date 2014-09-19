#pragma once

#include <string>

namespace Metabot
{
    class ComponentParameter
    {
        public:
            ComponentParameter(std::string name, std::string value, std::string description);
            virtual ~ComponentParameter();
            bool isBool();

            std::string name;
            std::string value;
            std::string description;
    };
}
