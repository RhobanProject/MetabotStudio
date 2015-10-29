#pragma once

#include <map>
#include <vector>
#include <string>
#include <3d/Model.h>
#include <json/json.h>
#include "Component.h"
#include "scad/Module.h"

namespace Metabot
{
    class Robot;
    class Cache;
    class Backend
    {
        public:
            static Backend *get(std::string name);

            Backend(std::string);
            virtual ~Backend();

            void load();
            void loadComponents(std::string directory);
            void loadComponent(std::string name);
            void buildCache();
            int clearCache();
            int cacheFiles();
            void parse(std::string path);

            Component *fromJson(Json::Value json, Robot *robot=NULL);
            Component *instanciate(std::string name);

            std::string name;
            std::string directory;

            Cache *cache;

            std::vector<Module> getModules();
            Module &getModule(std::string name);
            Model &getModel(std::string name);
            std::map<std::string, Model> models;

        protected:
            std::map<std::string, Module> modules;
    };
}
