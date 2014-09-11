#pragma once

#include <functional>
#include <map>
#include <vector>
#include <string>
#include <3d/Model.h>

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

            Model toModel();
            bool isCompatible(AnchorPoint *anchor);

            void compileAll();
            void compile();
#ifdef OPENGL
            void openGLDraw();
#endif

            std::string getValue(std::string name);
            std::string stl();
            std::string parameters();

            // Accessing parameter values
            std::string get(std::string name);
            void set(std::string name, std::string value);

            std::map<std::string, std::string> values;
            Component *component;
            
            std::vector<AnchorPoint *> anchors;
            std::vector<Part *> parts;
            std::vector<ModelRef *> models;

            Model myModel;
    };
}
