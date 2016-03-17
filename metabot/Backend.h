#pragma once

#include <map>
#include <mutex>
#include <vector>
#include <string>
#include <3d/Model.h>
#include <json/json.h>
#include "Component.h"
#include "scad/Module.h"

// Simulation mode
#define MODE_TORQUE     0
#define MODE_MOTORS     1

namespace Metabot
{
    class Robot;
    class Cache;
    class Backend
    {
        public:
            struct BackendMotor
            {
                double maxTorque;
                double maxSpeed;
            };

            struct BackendConfig
            {
                BackendConfig();
                int mode;
                double density;
                bool backlash;
                std::map<std::string, BackendMotor> motors;
                double getMaxTorque(std::string motor);
                double getMaxSpeed(std::string motor);
            };

            static Backend *get(std::string name);
            static void clean();

            Backend(std::string);
            virtual ~Backend();

            void load();
            void loadConfig(std::string filename);
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

            std::vector<Module*> getModules();
            Module *getModule(std::string name);
            Model &getModel(std::string name);
            std::map<std::string, Model> models;

            BackendConfig config;

        protected:
            std::map<std::string, Module*> modules;
    };
}
