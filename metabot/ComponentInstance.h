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
            
            float brightness;
#ifdef OPENGL
            void setBrightness(float brightness, bool recursive = false);
            void openGLDraw();
#endif
    
            // Merge the anchors from another entity
            // If detach is true, anchor will be detached from the other objects, and thus
            // "taken" from it
            void merge(ComponentInstance *other, bool detach);
            void detachAll();

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
