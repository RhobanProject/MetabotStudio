#include <sstream>
#include <iostream>
#include "Component.h"
#include "ComponentInstance.h"
#include "Cache.h"
#include "CSG.h"
#include "util.h"

namespace Metabot
{
    ComponentInstance::ComponentInstance(Component *component_)
        : component(component_)
    {
    }

    ComponentInstance::~ComponentInstance()
    {
        for (auto anchor : anchors) {
            delete anchor;
        }
        for (auto part : parts) {
            delete part;
        }
        for (auto model : models) {
            delete model;
        }
    }

    std::string ComponentInstance::get(std::string name)
    {
        return values[name];
    }

    void ComponentInstance::set(std::string name, std::string value)
    {
        values[name] = value;
    }

    void ComponentInstance::compile()
    {
        std::string csg = openscadCached("csg");
        std::string stl = openscadCached("stl");

        CSG *document = CSG::parse(csg);
        // XXX: todo: merge & release memory
        anchors = document->anchors;
        parts = document->parts;
        models = document->models;
        delete document;
    }
    
    std::string ComponentInstance::openscadCached(std::string filename, std::string format)
    {
        if (component->cache != NULL) {
            std::string key = hash_sha1(filename);

            return component->cache->get(key, [this, format, filename]() {
                return this->openscad(filename, format);
            });
        } else {
            return openscad(filename, format);
        }
    }
    
    std::string ComponentInstance::openscadCached(std::string format)
    {
        if (component->cache != NULL) {
            std::stringstream entry;
            entry << component->filename << "." << format << "/";
            for (auto value : values) {
                entry << value.first << "=" << value.second << "/";
            }
            std::string key = hash_sha1(entry.str());

            return component->cache->get(key, [this, format, component]() {
                return this->openscad(component->filename, format);
            });
        } else {
            return openscad(component->filename, format);
        }
    }

    std::string ComponentInstance::openscad(std::string filename, std::string format)
    {
        std::stringstream cmd;
        cmd << "openscad -DMotorMark=true ";
        for (auto value : values) {
            cmd << "-D" << value.first << "=" << value.second << " ";
        }
        std::string output = tempname() + "." + format;
        cmd << filename << " -o " << output << " >/dev/null 2>/dev/null";
        std::string command = cmd.str();
        
        // Uncomment that to see the compile command called
        // std::cout << "compile(): " << command << std::endl;

        FILE *process = popen(command.c_str(), "r");
        if (pclose(process) != 0) {
            std::stringstream error;
            error << "Compilation failed for part " << component->name;
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
