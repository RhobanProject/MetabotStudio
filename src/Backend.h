#pragma once

#include <vector>
#include <string>
#include "Component.h"

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

            std::string name;
            std::string directory;
            std::vector<Component *> components;
    };
}
