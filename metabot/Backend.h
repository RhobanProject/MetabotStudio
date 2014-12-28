#pragma once

#include <map>
#include <vector>
#include <string>
#include <3d/Model.h>
#include <json/json.h>
#include "Component.h"

namespace Metabot
{
    class Cache;
    class Backend
    {
        public:
            Backend(std::string);
            virtual ~Backend();

            void load();
            void loadComponents(std::string directory);
            void loadComponent(std::string name);
            void buildCache();
            int clearCache();
            int cacheFiles();
            void parse(std::string path);

            Component *getComponent(std::string name);
            ComponentInstance *fromJson(Json::Value json);

            std::string name;
            std::string directory;
            std::map<std::string, Component *> components;

            Cache *cache;

            Model getModel(std::string name);
            std::map<std::string, Model> models;
            
            std::string openscad(std::string filename, std::string format, std::string parameters="");
            std::string doOpenscad(std::string filename, std::string format, std::string parameters="");
    };
}
