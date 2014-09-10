#include <iostream>
#include <sstream>
#include <dirent.h>
#include <3d/stl.h>
#include "Backend.h"
#include "Cache.h"
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

    Model Backend::getModel(std::string name)
    {
        if (!models.count(name)) {
            std::string filename = directory + "/models/" + name + ".scad";
            Model model = loadModelSTL_string(openscadCached(filename, "stl"));
            models[name] = model;
        }

        return models[name];
    }
   
    std::string Backend::openscadCached(std::string filename, std::string format, std::string parameters)
    {
        std::string key = hash_sha1(filename + "." + format + " w/ " + parameters);
        if (cache != NULL) {
            return cache->get(key, [this, format, filename, parameters]() {
                return this->openscad(filename, format, parameters);
            });
        } else {
            return openscad(filename, format);
        }
    }
    
    std::string Backend::openscad(std::string filename, std::string format, std::string parameters)
    {
        std::stringstream cmd;
        cmd << "openscad -D\\$fn=20 ";
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
}
