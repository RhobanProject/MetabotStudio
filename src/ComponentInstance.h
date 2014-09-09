#pragma once

#include <map>
#include <vector>
#include <string>
#include "AnchorPoint.h"

namespace Metabot
{
    class Component;
    class ComponentInstance
    {
        public:
            ComponentInstance(Component *component);

            void compile();
            std::string openscadCached(std::string format);
            std::string openscad(std::string format);

            // Accessing parameter values
            std::string get(std::string name);
            void set(std::string name, std::string value);

            std::map<std::string, std::string> values;
            std::vector<AnchorPoint *> anchors;
            Component *component;
    };
}
