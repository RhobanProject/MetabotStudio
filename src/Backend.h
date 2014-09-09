#pragma once

#include <map>
#include <vector>
#include <string>
#include "Component.h"
#include "Cache.h"

namespace Metabot
{
    class Backend
    {
        public:
            Backend(std::string);
            virtual ~Backend();

            void load();
            void loadComponents();
            void loadComponent(std::string name);

            std::vector<Component*> getComponents(std::string type);

            std::string name;
            std::string directory;
            std::map<std::string, Component *> components;

            Cache cache;
    };
}
