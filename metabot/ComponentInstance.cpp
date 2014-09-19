#include <sstream>
#include <iostream>
#include <3d/stl.h>
#ifdef OPENGL
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#endif
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
            
    bool ComponentInstance::isCompatible(AnchorPoint *anchor)
    {
        for (auto my : anchors) {
            if (my->isCompatible(anchor)) {
                return true;
            }
        }

        return false;
    }
            
#ifdef OPENGL
    void ComponentInstance::openGLDraw(bool highlight)
    {
        if (highlight) {
            myModel.r = 0.4;
            myModel.g = 1.0;
            myModel.b = 0.3;
        } else {
            myModel.r = 0.9;
            myModel.g = 0.9;
            myModel.b = 0.9;
        }
        myModel.openGLDraw();

        // Rendering models
        for (auto ref : models) {
            glPushMatrix();
            ref->matrix.openGLMult();
            Model model = component->backend->getModel(ref->name);
            model.r = ref->r;
            model.g = ref->g;
            model.b = ref->b;
            model.openGLDraw();
            glPopMatrix();
        }

        // Rendering sub-components
        for (auto anchor : anchors) {
            glPushMatrix();
            if (anchor->above) {
                anchor->openGLDraw();
            }
            glPopMatrix();
        }
    }
            
    void ComponentInstance::unHighlight()
    {
        for (auto anchor : anchors) {
            anchor->highlight = false;
            if (anchor->anchor != NULL && anchor->above) {
                anchor->anchor->instance->unHighlight();
            }
        }
    }
#endif

    Model ComponentInstance::toModel()
    {
        Model model = myModel;

        // Rendering models
        for (auto ref : models) {
            Model m = component->backend->getModel(ref->name);
            m.apply(ref->matrix);
            model.merge(m);
        }

        // Rendering sub-components
        for (auto anchor : anchors) {
            if (anchor->above) {
                Model component = anchor->toModel();
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
        std::string csg = component->backend->openscad(filename, "csg", parameters());
        myModel = loadModelSTL_string(stl());

        CSG *document = CSG::parse(csg);
        // XXX: todo: merge & release memory
        anchors = document->anchors;
        parts = document->parts;
        models = document->models;

        int index = 1;
        for (auto anchor : anchors) {
            anchor->instance = this;
            anchor->id = index;
            index++;
        }

        delete document;
    }

    void ComponentInstance::merge(ComponentInstance *other, bool detach)
    {
        for (unsigned int i=0; i<anchors.size(); i++) {
            if (i < other->anchors.size()) {
                AnchorPoint *myAnchor = anchors[i];
                AnchorPoint *otherAnchor = other->anchors[i];

                if (otherAnchor->anchor && myAnchor->isCompatible(otherAnchor->anchor)) {
                    myAnchor->anchor = otherAnchor->anchor;
                    myAnchor->anchor->anchor = myAnchor;
                    myAnchor->above = otherAnchor->above;
                    
                    if (detach) {
                        otherAnchor->detach(false);
                    }
                }
            }
        }
    }
            
    void ComponentInstance::restore()
    {
        for (auto anchor : anchors) {
            if (anchor->anchor) {
                anchor->anchor->anchor = anchor;
            }
        }
    }

    void ComponentInstance::detachAll()
    {
        for (auto anchor : anchors) {
            anchor->detach(false);
        }
    }
            
    std::string ComponentInstance::getValue(std::string name)
    {
        if (values.count(name)) {
            return values[name];
        }

        return "";
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

    std::string ComponentInstance::stl()
    {
        return component->backend->openscad(component->filename, "stl", parameters());
    }

    Json::Value ComponentInstance::parametersJson()
    {
        Json::Value json(Json::objectValue);

        for (auto value : values) {
            json[value.first] = value.second;
        }

        return json;
    }
            
    AnchorPoint *ComponentInstance::getAnchor(int id)
    {
        for (auto anchor : anchors) {
            if (anchor->id == id) {
                return anchor;
            }
        }

        return NULL;
    }
            
    void ComponentInstance::parametersFromJson(Json::Value json)
    {
        for (auto name : json.getMemberNames()) {
            values[name] = json[name].asString();
        }
    }
            
    Json::Value ComponentInstance::toJson()
    {
        Json::Value json(Json::objectValue);

        json["component"] = component->name;
        json["parameters"] = parametersJson();
        json["anchors"] = Json::Value(Json::objectValue);

        for (auto anchor : anchors) {
            if (anchor->anchor != NULL && anchor->above) {
                std::stringstream ss;
                ss << anchor->id;
                std::string id = ss.str();
                json["anchors"][id] = Json::Value(Json::objectValue);
                json["anchors"][id]["zero"] = anchor->zero;
                json["anchors"][id]["remote"] = anchor->anchor->id;
                json["anchors"][id]["component"] = anchor->anchor->instance->toJson();
            }
        }

        return json;
    }
            
    void ComponentInstance::foreach(std::function<void(ComponentInstance *instance)> method)
    {
        method(this);

        for (auto anchor : anchors) {
            if (anchor->anchor != NULL && anchor->above) {
                anchor->anchor->instance->foreach(method);
            }
        }
    }
            
    void ComponentInstance::foreachAnchor(std::function<void(AnchorPoint *instance)> method)
    {
        for (auto anchor : anchors) {
            if (anchor->anchor != NULL && anchor->above) {
                if (anchor->type != "root") {
                    method(anchor);
                }

                anchor->anchor->instance->foreachAnchor(method);
            }
        }
    }
}
