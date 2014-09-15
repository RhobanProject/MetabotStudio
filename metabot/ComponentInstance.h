#pragma once

#include <functional>
#include <map>
#include <vector>
#include <string>
#include <3d/Model.h>
#include <json/json.h>

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

            Json::Value parametersJson();
            void parametersFromJson(Json::Value json);
            Json::Value toJson();

            void compileAll();
            void compile();
            
#ifdef OPENGL
            void unHighlight();
            void openGLDraw(bool highlight = false);
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

            AnchorPoint *getAnchor(int id);
            
            std::vector<AnchorPoint *> anchors;
            std::vector<Part *> parts;
            std::vector<ModelRef *> models;

            Model myModel;
    };
}
