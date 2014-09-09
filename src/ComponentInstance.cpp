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
                Model m = loadModelSTL_string(component->backend->openscad(filename, "stl"));
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
        std::string filename = component->filename;
        std::string csg = component->backend->openscadCached(csgHash(), filename, "csg", parameters());
        component->backend->openscadCached(stlHash(), filename, "stl", parameters());

        CSG *document = CSG::parse(csg);
        // XXX: todo: merge & release memory
        anchors = document->anchors;
        parts = document->parts;
        models = document->models;
        delete document;
    }

    std::string ComponentInstance::parameters()
    {
        std::stringstream cmd;
        cmd << "-DNoModels=true ";
        for (auto value : values) {
            cmd << "-D" << value.first << "=" << value.second << " ";
        }
        return cmd.str();
    }

    std::string ComponentInstance::csgHash()
    {
        return hash_sha1(component->filename+".csg / "+parameters());
    }

    std::string ComponentInstance::stlHash()
    {
        return hash_sha1(component->filename+".stl / "+parameters());
    }

    std::string ComponentInstance::stl()
    {
        return component->backend->openscadCached(stlHash(), component->filename, "stl", parameters());
    }
}
