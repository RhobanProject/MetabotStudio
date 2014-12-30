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
            bool isString();
            void updateValue(std::string value);
            std::string getValue();

            std::string name;
            std::string value;
            std::string description;
    };
}
