#include <iostream>
#include <sstream>
#include <dirent.h>
#include "Backend.h"
#include "Cache.h"

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
            
    std::vector<Component*> Backend::getComponents(std::string type)
    {
        std::vector<Component *> all;
        for (auto component : components) {
            if (component.second->type == type) {
                all.push_back(component.second);
            }
        }
        return all;
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

    bool Backend::hasModel(std::string name)
    {
        return models.count(name);
    }

    void Backend::setModel(std::string name, Model m)
    {
        models[name] = m;
    }

    Model Backend::getModel(std::string name)
    {
        return models[name];
    }
}
