#pragma once

#include <vector>
#include <string>
#include "ComponentParameter.h"

namespace Metabot
{
    class Component
    {
        public:
            Component(std::string name);
            virtual ~Component();

            std::string name, type;
            std::vector<ComponentParameter *> parameters;
            
            static Component *load(std::string filename);
    };
}
