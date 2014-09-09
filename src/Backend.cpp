#include <iostream>
#include <sstream>
#include <dirent.h>
#include "Backend.h"

namespace Metabot
{
    Backend::Backend(std::string name_)
        : name(name_)
    {
        directory = "backends/" + name;
        std::string cacheDir = directory + "/cache";
        cache.setDirectory(cacheDir);
    }

    Backend::~Backend()
    {
        for (auto component : components) {
            delete component.second;
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
                component->cache = &cache;
                components[component->name] = component;
            }
        }
    }            
}
