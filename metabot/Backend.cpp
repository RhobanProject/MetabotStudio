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
    Backend::Backend(std::string name_)
        : name(name_)
    {
        directory = "backends/" + name;
        std::string cacheDir = directory + "/cache";
        cache = new Cache();
        cache->setDirectory(cacheDir);
    }

    Backend::~Backend()
    {
        if (cache != NULL) {
            delete cache;
        }
    }
            
    void Backend::buildCache()
    {
        for (auto module : modules) {
            if (module.second.getType() == "component") {
                // Creating component
                auto component = instanciate(module.second.getName());
                component->compile();
                delete component;
            }
        }
        getModel("anchor");
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

        for (auto module : scadModules) {
            module.setBackend(this);
            modules[module.getName()] = module;
        }
    }
            
    Module &Backend::getModule(std::string name)
    {
        return modules[name];
    }

    std::vector<Module> Backend::getModules()
    {
        std::vector<Module> allModules;

        for (auto entry : modules) {
            allModules.push_back(entry.second);
        }

        return allModules;
    }

    Model Backend::getModel(std::string name)
    {
        if (!models.count(name)) {
            if (modules.count(name)) {
                Model model = loadModelSTL_string(getModule(name).openscad("stl"));
                models[name] = model;
            } else {
                return Model();
            }
        }

        return models[name];
    }
            
    Component *Backend::instanciate(std::string name)
    {
        if (modules.count(name)) {
            Module *module = &modules[name];
            Component *component = new Component(this, module);

            return component;
        }

        return NULL;
    }

    Component *Backend::fromJson(Json::Value json)
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
        instance->compile();

        Json::Value &anchors = json["anchors"];
        for (auto anchor : instance->anchors) {
            std::stringstream ss;
            ss << anchor->id;
            std::string id = ss.str();
            if (anchors.isMember(id)) {
                int remote = anchors[id]["remote"].asInt();
                Component *instance = Backend::fromJson(anchors[id]["component"]);
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
                anchor->attach(instance->getAnchor(remote));
            }
        }

        return instance;
    }
}
