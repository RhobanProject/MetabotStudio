#include <sstream>
#include <iostream>
#include <3d/stl.h>
#include "Component.h"
#include "ComponentInstance.h"
#include "AnchorPoint.h"
#include "Part.h"
#include "ModelRef.h"
#include "Backend.h"
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
    }

    void ComponentInstance::computeModel()
    {
        myModel = loadModelSTL_string(stl());
        
        for (auto anchor : anchors) {
            if (anchor->instance != NULL) {
                anchor->instance->computeModel();
            }
        }
    }

    Model ComponentInstance::toModel()
    {
        Model model = myModel;

        // Rendering models
        for (auto ref : models) {
            if (!component->backend->hasModel(ref->name)) {
                std::string filename = component->backend->directory + "/models/" + ref->name + ".scad";
                Model m = loadModelSTL_string(openscadCached(filename, "stl"));
                component->backend->setModel(ref->name, m);
            }
            Model m = component->backend->getModel(ref->name);
            m.apply(ref->matrix);
            model.merge(m);
        }

        // Rendering sub-components
        for (auto anchor : anchors) {
            if (anchor->instance != NULL) {
                Model component = anchor->toModel();
                component.apply(anchor->matrix);
                model.merge(component);
            }
        }

        return model;
    }

    std::string ComponentInstance::get(std::string name)
    {
        return values[name];
    }

    void ComponentInstance::set(std::string name, std::string value)
    {
        values[name] = value;
    }

    void ComponentInstance::compileAll()
    {
        compile();

        for (auto anchor : anchors) {
            if (anchor->instance != NULL) {
                anchor->instance->compileAll();
            }
        }
    }

    void ComponentInstance::compile()
    {
        std::string csg = openscadCached("csg");
        openscadCached("stl");

        CSG *document = CSG::parse(csg);
        // XXX: todo: merge & release memory
        anchors = document->anchors;
        parts = document->parts;
        models = document->models;
        delete document;
    }

    std::string ComponentInstance::stl()
    {
        return openscadCached("stl");
    }
    
    std::string ComponentInstance::openscadCached(std::string filename, std::string format)
    {
        if (component->backend->cache != NULL) {
            std::string key = hash_sha1(filename);

            return component->backend->cache->get(key, [this, format, filename]() {
                return this->openscad(filename, format);
            });
        } else {
            return openscad(filename, format);
        }
    }
    
    std::string ComponentInstance::openscadCached(std::string format)
    {
        std::stringstream cmd;
        cmd << "-DNoModels=true ";
        for (auto value : values) {
            cmd << "-D" << value.first << "=" << value.second << " ";
        }
        std::string parameters = cmd.str();

        if (component->backend->cache != NULL) {
            std::stringstream entry;
            entry << component->filename << "." << format << " w/ ";
            entry << parameters;
            std::string key = hash_sha1(entry.str());

            return component->backend->cache->get(key, [this, format, parameters, component]() {
                return this->openscad(component->filename, format, parameters);
            });
        } else {
            return openscad(component->filename, format, parameters);
        }
    }

    std::string ComponentInstance::openscad(std::string filename, std::string format, std::string parameters)
    {
        std::stringstream cmd;
        cmd << "openscad ";
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
