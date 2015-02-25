#pragma once

#include <string>
#include <map>
#include "Parameter.h"

namespace Metabot
{
    class Parameters : public std::map<std::string, Parameter>
    {
        public:
            Parameters();

            void set(std::string name, std::string value);
            void update(std::string name, std::string value);
            Parameter &get(std::string name);
            bool has(std::string name);

            std::string toArgs();
    };
}
