#pragma once

#include <string>

namespace Metabot
{
    class Parameter
    {
        public:
            Parameter();
            Parameter(std::string name, std::string value="", std::string description="");
            virtual ~Parameter();
            bool isBool();

            std::string name;
            std::string value;
            std::string description;
    };
}
