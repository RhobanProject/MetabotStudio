#pragma once

#include <map>
#include <vector>
#include <string>
#include "ComponentParameter.h"

namespace Metabot
{
    class Cache;
    class ComponentInstance;
    class Component
    {
        public:
            Component(std::string name, std::string filename);
            virtual ~Component();

            ComponentInstance *instanciate();

            std::string name, filename, type;
            std::map<std::string, ComponentParameter *> parameters;
            
            Cache *cache;
            static Component *load(std::string filename);
    };
}
