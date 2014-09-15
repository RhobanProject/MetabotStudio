#include <iostream>
#include <sstream>
#include <dirent.h>
#include <3d/stl.h>
#include "Backend.h"
#include "Cache.h"
#include "ComponentInstance.h"
#include "AnchorPoint.h"
#include "ModelRef.h"
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
        for (auto component : components) {
            delete component.second;
        }

        if (cache != NULL) {
            delete cache;
        }
    }
            
    void Backend::buildCache()
    {
        for (auto component : components) {
            auto instance = component.second->instanciate();
            instance->compile();
            for (auto ref : instance->models) {
                getModel(ref->name);
            }
            delete instance;
        }
    }
            
    Component *Backend::getComponent(std::string name)
    {
        if (components.count(name)) {
            return components[name];
        }

        return NULL;
    }

    void Backend::load()
    {
        DIR *test;
        test = opendir(directory.c_str());
        if (test != NULL) {
            closedir(test);
            loadComponents();
        } else {
            std::stringstream s;
            s << "Unable to open directory: " << directory;
            throw s.str();
        }
    }

    void Backend::loadComponents()
    {
        DIR *dir;
        struct dirent *ent;
        std::string dirname = directory + "/components";
        if ((dir = opendir(dirname.c_str())) != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                loadComponent(std::string(ent->d_name));
            }
        } else {
            std::stringstream s;
            s << "No components/ in backend " << name;
            throw s.str();
        }

        if (components.size() == 0) {
            throw std::string("There is no component");
        }
    }

    void Backend::loadComponent(std::string name)
    {
        std::string filename = directory + "/components/" + name;
        if (filename.length()>5 && filename.substr(filename.length()-5)==".scad") {
            Component *component = Component::load(filename);
            if (component != NULL) {
                component->backend = this;
                components[component->name] = component;
            }
        }
    }            

    Model Backend::getModel(std::string name)
    {
        if (!models.count(name)) {
            std::string filename = directory + "/models/" + name + ".scad";
            Model model = loadModelSTL_string(openscad(filename, "stl"));
            models[name] = model;
        }

        return models[name];
    }
   
    std::string Backend::openscad(std::string filename, std::string format, std::string parameters)
    {
        std::string key = hash_sha1(filename + "." + format + " w/ " + parameters);
        if (cache != NULL) {
            return cache->get(key, [this, format, filename, parameters]() {
                return this->doOpenscad(filename, format, parameters);
            });
        } else {
            return doOpenscad(filename, format);
        }
    }
    
    std::string Backend::doOpenscad(std::string filename, std::string format, std::string parameters)
    {
        std::stringstream cmd;
        cmd << "openscad -D\\$fn=15 ";
        cmd << parameters;
        std::string output = tempname() + "." + format;
        cmd << filename << " -o " << output;
        // cmd << " >/dev/null 2>/dev/null";
        std::string command = cmd.str();
        
        // Uncomment that to see the compile command called
        std::cout << "compile(): " << command << std::endl;

        FILE *process = popen(command.c_str(), "r");
        if (pclose(process) != 0) {
            std::stringstream error;
            error << "Compilation failed for file " << filename;
            throw error.str();
        }

        if (!file_exists(output)) {
            std::stringstream error;
            error << "Compilation did not produced the output file";
            throw error.str();
        }

        std::string data = file_get_contents(output);
        remove(output.c_str());

        return data;
    }

    ComponentInstance *Backend::fromJson(Json::Value json)
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

        if (!components.count(component)) {
            std::stringstream ss;
            ss << "Unknown component " << component;
            throw ss.str();
        }

        ComponentInstance *instance = getComponent(component)->instanciate();
        instance->parametersFromJson(json["parameters"]);
        instance->compile();

        Json::Value &anchors = json["anchors"];
        for (auto anchor : instance->anchors) {
            std::stringstream ss;
            ss << anchor->id;
            std::string id = ss.str();
            if (anchors.isMember(id)) {
                int remote = anchors[id]["remote"].asInt();
                ComponentInstance *instance = Backend::fromJson(anchors[id]["component"]);
                anchor->attach(instance->getAnchor(remote));
            }
        }

        return instance;
    }
}
