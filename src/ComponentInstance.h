#pragma once

#include <map>
#include <vector>
#include <string>

namespace Metabot
{
    class AnchorPoint;
    class Part;
    class ModelRef;
    class Component;
    class ComponentInstance
    {
        public:
            ComponentInstance(Component *component);
            virtual ~ComponentInstance();

            void compile();
            std::string openscadCached(std::string filename, std::string format);
            std::string openscadCached(std::string format);
            std::string openscad(std::string filename, std::string format);

            // Accessing parameter values
            std::string get(std::string name);
            void set(std::string name, std::string value);

            std::map<std::string, std::string> values;
            Component *component;
            
            std::vector<AnchorPoint *> anchors;
            std::vector<Part *> parts;
            std::vector<ModelRef*> models;
    };
}
