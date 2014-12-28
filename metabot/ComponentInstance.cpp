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
#include "Parts.h"
#include "ModelRefs.h"
#include "Backend.h"
#include "Cache.h"
#include "CSG.h"
#include "util.h"

namespace Metabot
{
    ComponentInstance::ComponentInstance(Component *component_)
        : component(component_), highlight(false), hover(false)
    {
    }

    ComponentInstance::~ComponentInstance()
    {
        for (auto anchor : anchors) {
            if (anchor->above) {
                delete anchor;
            }
        }
    }

    ComponentInstance *ComponentInstance::clone()
    {
        ComponentInstance *instance = new ComponentInstance(component);
        instance->myModel = myModel;
        instance->values = values;

        int index = 0;
        for (auto anchor : anchors) {
            AnchorPoint *anchorPoint = anchor->clone();
            anchorPoint->instance = instance;
            anchorPoint->id = index++;
            instance->anchors.push_back(anchorPoint);

            AnchorPoint *remote = anchor->anchor;
            if (anchor->above && remote != NULL) {
                ComponentInstance *child = remote->instance->clone();
                anchorPoint->attach(child->anchors[remote->id]);
            }
        }

        instance->models = models;
        instance->parts = parts;
        instance->bom = bom;

        return instance;
    }

    void ComponentInstance::root()
    {
        for (auto anchor : anchors) {
            if (anchor->above == false) {
                anchor->revert();
            }
        }
    }

    std::string ComponentInstance::fullName()
    {
        std::stringstream ss;
        ss << component->name << " #" << id;
        return ss.str();
    }

    AnchorPoint *ComponentInstance::findCompatible(AnchorPoint *anchor)
    {
        for (auto my : anchors) {
            if (my->isCompatible(anchor) && my->anchor==NULL) {
                return my;
            }
        }

        return NULL;
    }

    bool ComponentInstance::isCompatible(AnchorPoint *anchor)
    {
        return findCompatible(anchor) != NULL;
    }

#ifdef OPENGL
    void ComponentInstance::openGLDraw()
    {
        glStencilFunc(GL_ALWAYS, id, -1);

        if (highlight) {
            myModel.r = 0.4;
            myModel.g = 1.0;
            myModel.b = 0.3;
        } else {
            myModel.r = 0.95;
            myModel.g = 0.95;
            myModel.b = 0.95;
        }
        myModel.openGLDraw();

        // Rendering models
        for (auto ref : models.models) {
            glPushMatrix();
            ref.matrix.openGLMult();
            Model model = component->backend->getModel(ref.name);
            if (highlight) {
                model.r = 0.4;
                model.g = 1.0;
                model.b = 0.3;
            } else {
                model.r = ref.r;
                model.g = ref.g;
                model.b = ref.b;
            }
            model.openGLDraw();
            glPopMatrix();
        }

        // Rendering sub-components
        int anchorId = 1;
        for (auto anchor : anchors) {
            glPushMatrix();
            if (anchor->above) {
                anchor->openGLDraw(anchorId);
            }
            glPopMatrix();
            anchorId++;
        }
    }
#endif

    Model ComponentInstance::toModel()
    {
        Model model;
        model = myModel;

        // Rendering models
        for (auto ref : models.models) {
            Model m = component->backend->getModel(ref.name);
            m.apply(ref.matrix);
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
        anchors = document->anchors;
        parts = document->parts;
        models = document->models;
        bom = document->bom;

        int index = 0;
        for (auto anchor : anchors) {
            anchor->instance = this;
            anchor->id = index;
            index++;
        }

        delete document;
    }

    void ComponentInstance::moveAnchors(ComponentInstance *other)
    {
        // First step:
        // Trying to take the items that matches *exactly* the anchor from the
        // old component
        for (unsigned int i=0; i<anchors.size(); i++) {
            if (i < other->anchors.size()) {
                AnchorPoint *myAnchor = anchors[i];
                AnchorPoint *otherAnchor = other->anchors[i];

                if (otherAnchor->anchor && myAnchor->isCompatible(otherAnchor->anchor)) {
                    myAnchor->copyData(otherAnchor);
                    myAnchor->attach(otherAnchor->anchor);
                    otherAnchor->detach(false);
                }
            }
        }

        // Second step:
        // Trying to get the anchors 
        for (auto anchor : other->anchors) {
            AnchorPoint *remote = anchor->anchor;
            if (remote != NULL) {
                AnchorPoint *candidate = findCompatible(remote);

                if (candidate != NULL) {
                    candidate->copyData(remote);
                    candidate->attach(remote);
                    anchor->detach(false);
                }
            }
        }
    }

    void ComponentInstance::detachDiffAnchors(ComponentInstance *other)
    {
        std::map<AnchorPoint *, bool> otherAnchors;

        for (auto anchor : other->anchors) {
            if (anchor->anchor) {
                otherAnchors[anchor->anchor] = true;
            }
        }

        for (auto anchor : anchors) {
            if (otherAnchors.count(anchor)) {
                anchor->detach();
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
            
    AnchorPoint *ComponentInstance::belowAnchor()
    {
        for (auto anchor : anchors) {
            if (anchor->above == false && anchor->anchor) {
                return anchor;
            }
        }

        return NULL;
    }

    AnchorPoint *ComponentInstance::aboveAnchor()
    {
        AnchorPoint *below = belowAnchor();
        if (below != NULL) {
            return below->anchor;
        }

        return NULL;
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
                json["anchors"][id]["orientationX"] = anchor->orientationX;
                json["anchors"][id]["orientationY"] = anchor->orientationY;
                json["anchors"][id]["orientationZ"] = anchor->orientationZ;
                json["anchors"][id]["remote"] = anchor->anchor->id;
                json["anchors"][id]["component"] = anchor->anchor->instance->toJson();
            }
        }

        return json;
    }

    void ComponentInstance::foreachComponent(std::function<void(ComponentInstance *instance)> method)
    {
        method(this);

        for (auto anchor : anchors) {
            if (anchor->anchor != NULL && anchor->above) {
                anchor->anchor->instance->foreachComponent(method);
            }
        }
    }

    void ComponentInstance::foreachAnchor(std::function<void(AnchorPoint *instance)> method)
    {
        for (auto anchor : anchors) {
            if (anchor->anchor != NULL && anchor->above) {
                method(anchor);
                anchor->anchor->instance->foreachAnchor(method);
            }
        }
    }

    void ComponentInstance::onHover()
    {
        hover = true;
        for (auto anchor : anchors) {
            anchor->hover = true;
        }
    }
}
