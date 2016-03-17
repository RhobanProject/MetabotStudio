#include <iostream>
#include <sstream>
#include <3d/stl.h>
#include "Backend.h"
#include "Cache.h"
#include "Component.h"
#include "AnchorPoint.h"
#include "SCAD.h"
#include "util.h"

namespace Metabot
{
    static std::mutex mutex;
    static std::map<std::string, Backend *> backends;

    Backend::BackendConfig::BackendConfig()
        : mode(MODE_TORQUE),
        density(1.25),
        backlash(true)
    {
    }

    double Backend::BackendConfig::getMaxTorque(std::string motor)
    {
        if (motors.count(motor)) {
            return motors[motor].maxTorque;
        }

        return 0.5;
    }

    double Backend::BackendConfig::getMaxSpeed(std::string motor)
    {
        if (motors.count(motor)) {
            return motors[motor].maxSpeed;
        }

        return 4*M_PI;
    }

    Backend *Backend::get(std::string name)
    {
        mutex.lock();
        if (!backends.count(name)) {
            backends[name] = new Backend(name);
            backends[name]->load();
        }

        auto backend = backends[name];
        mutex.unlock();

        return backend;
    }

    void Backend::clean()
    {
        for (auto backend : backends) {
            delete backend.second;
        }
        backends.clear();
    }

    Backend::Backend(std::string name_)
        : name(name_)
    {
        directory = "backends/" + name;
        std::string cacheDir = directory + "/cache";
        cache = new Cache();
        cache->setDirectory(cacheDir);

        std::string configFile = directory+"/config.json";
        if (file_exists(configFile)) {
            loadConfig(configFile);
        }
    }

    void Backend::loadConfig(std::string filename)
    {
        std::string data = file_get_contents(filename);
        Json::Reader reader;
        Json::Value json;

        if (reader.parse(data, json)) {
            if (json.isMember("backlash")) {
                config.backlash = json["backlash"].asBool();
            }
            if (json.isMember("mode")) {
                if (json["mode"].asString() == "motor") {
                    config.mode = MODE_MOTORS;
                } else {
                    config.mode = MODE_TORQUE;
                }
            }
            if (json.isMember("density")) {
                config.density = json["density"].asDouble();
            }
            if (json.isMember("motors")) {
                for (auto &entry : json["motors"].getMemberNames()) {
                    auto &motor = json["motors"][entry];
                    if (motor.isMember("maxTorque")) {
                        config.motors[entry].maxTorque = motor["maxTorque"].asDouble();
                    }
                    if (motor.isMember("maxSpeed")) {
                        config.motors[entry].maxSpeed = motor["maxSpeed"].asDouble();
                    }
                }
            }
        }
    }

    Backend::~Backend()
    {
        if (cache != NULL) {
            delete cache;
        }
        for (auto module : modules) {
            delete module.second;
        }
    }

    void Backend::buildCache()
    {
        for (auto module : modules) {
            if (module.second->getType() == "component") {
                // Creating component
                auto component = instanciate(module.first);
                component->compile();
                delete component;
            }
        }
        getModel("anchor");
        getModel("com");
    }

    int Backend::clearCache()
    {
        return cache->clear();
    }

    int Backend::cacheFiles()
    {
        return cache->files();
    }

    void Backend::load()
    {
        if (is_directory(directory)) {
            loadComponents(directory);
        } else {
            std::stringstream s;
            s << "Unable to open directory: " << directory;
            throw s.str();
        }
    }

    void Backend::loadComponents(std::string dir)
    {
        auto listing = get_directory_listing(dir);

        for (auto entry : listing) {
            std::string fullPath = dir+"/"+entry;
            if (is_directory(fullPath)) {
                loadComponents(fullPath);
            } else {
                if (endswith(fullPath, ".scad") && !endswith(fullPath, ".metabot.scad")) {
                    parse(fullPath);
                }
            }
        }
    }

    void Backend::parse(std::string path)
    {
        auto scadModules = SCAD::load(path);

        for (auto &module : scadModules) {
            auto mod = new Module;
            *mod = module;
            mod->setBackend(this);
            modules[module.getName()] = mod;
        }
    }

    Module *Backend::getModule(std::string name)
    {
        return modules[name];
    }

    std::vector<Module*> Backend::getModules()
    {
        std::vector<Module*> allModules;

        for (auto module : modules) {
            allModules.push_back(module.second);
        }

        return allModules;
    }

    Model &Backend::getModel(std::string name)
    {
        if (!models.count(name)) {
            if (modules.count(name)) {
                Model model = loadModelSTL_string(getModule(name)->openscad("stl"));
                models[name] = model;
            } else {
                models[name] = Model();
            }
        }

        return models[name];
    }

    Component *Backend::instanciate(std::string name)
    {
        if (modules.count(name)) {
            Component *component = new Component(this, modules[name]);

            return component;
        }

        return NULL;
    }

    Component *Backend::fromJson(Json::Value json, Robot *robot)
    {
        if (!json.isObject() || !json.isMember("component") 
                || !json.isMember("parameters") || !json.isMember("anchors")
                || !json["component"].isString() || !json["parameters"].isObject()
                || !json["anchors"].isObject()) {
            std::stringstream ss;
            ss << "Malformed Json component";
            throw ss.str();
        }

        std::string component = json["component"].asString();

        if (!modules.count(component)) {
            std::stringstream ss;
            ss << "Unknown component " << component;
            throw ss.str();
        }

        Component *instance = instanciate(component);
        instance->parametersFromJson(json["parameters"]);
        instance->compile(robot);

        Json::Value &anchors = json["anchors"];
        for (auto anchor : instance->anchors) {
            std::stringstream ss;
            ss << anchor->id;
            std::string id = ss.str();
            if (anchors.isMember(id)) {
                int remote = anchors[id]["remote"].asInt();
                Component *instance = Backend::fromJson(anchors[id]["component"], robot);
                if (anchors[id].isMember("zero")) {
                    anchor->zero = anchors[id]["zero"].asFloat();
                }
                if (anchors[id].isMember("orientationX")) {
                    anchor->orientationX = anchors[id]["orientationX"].asFloat();
                }
                if (anchors[id].isMember("orientationY")) {
                    anchor->orientationY = anchors[id]["orientationY"].asFloat();
                }
                if (anchors[id].isMember("orientationZ")) {
                    anchor->orientationZ = anchors[id]["orientationZ"].asFloat();
                }
                if (anchors[id].isMember("minimum")) {
                    anchor->minimum = anchors[id]["minimum"].asFloat();
                }
                if (anchors[id].isMember("maximum")) {
                    anchor->maximum = anchors[id]["maximum"].asFloat();
                }
                anchor->attach(instance->getAnchor(remote));
            }
        }

        return instance;
    }
}
