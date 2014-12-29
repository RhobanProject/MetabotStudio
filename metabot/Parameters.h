#pragma once

#include <string>
#include <map>
#include "Parameter.h"

namespace Metabot
{
    class Parameters
    {
        public:
            Parameters();

            void set(std::string name, std::string value);
            Parameter &get(std::string name);
            std::map<std::string, Parameter> &getAll();

            std::string toArgs();

        protected:
            std::map<std::string, Parameter> parameters;
    };
}
