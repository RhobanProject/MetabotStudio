#pragma once

#include <map>
#include <vector>
#include <string>
#include "ComponentParameter.h"

namespace Metabot
{
    class Backend;
    class ComponentInstance;
    class Component
    {
        public:
            Component(std::string name, std::string filename);
            virtual ~Component();

            ComponentInstance *instanciate();

            std::string name, filename, type;
            std::string prettyName, description;
            std::map<std::string, ComponentParameter *> parameters;
           
            Backend *backend;
            static Component *load(std::string filename);
    };
}
