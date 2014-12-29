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
#include "AnchorPoint.h"
#include "Parts.h"
#include "ModelRefs.h"
#include "Backend.h"
#include "Cache.h"
#include "CSG.h"
#include "util.h"

namespace Metabot
{
    Component::Component(Backend *backend_, Module *module_)
        : backend(backend_), module(module_), highlight(false), hover(false)
    {
    }

    Component::~Component()
    {
        for (auto anchor : anchors) {
            if (anchor->above) {
                delete anchor;
            }
        }
    }

    Component *Component::clone()
    {
        Component *component = new Component(backend, module);
        component->myModel = myModel;
        component->values = values;

        int index = 0;
        for (auto anchor : anchors) {
            AnchorPoint *anchorPoint = anchor->clone();
            anchorPoint->component = component;
            anchorPoint->id = index++;
            component->anchors.push_back(anchorPoint);

            AnchorPoint *remote = anchor->anchor;
            if (anchor->above && remote != NULL) {
                Component *child = remote->component->clone();
                anchorPoint->attach(child->anchors[remote->id]);
            }
        }

        component->models = models;
        component->parts = parts;
        component->bom = bom;

        return component;
    }

    void Component::root()
    {
        for (auto anchor : anchors) {
            if (anchor->above == false) {
                anchor->revert();
            }
        }
    }

    std::string Component::fullName()
    {
        std::stringstream ss;
        ss << module->getName() << " #" << id;
        return ss.str();
    }

    AnchorPoint *Component::findCompatible(AnchorPoint *anchor)
    {
        for (auto my : anchors) {
            if (my->isCompatible(anchor) && my->anchor==NULL) {
                return my;
            }
        }

        return NULL;
    }

    bool Component::isCompatible(AnchorPoint *anchor)
    {
        return findCompatible(anchor) != NULL;
    }

#ifdef OPENGL
    void Component::openGLDraw()
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
            Model model = backend->getModel(ref.name);
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

    Model Component::toModel()
    {
        Model model;
        model = myModel;

        // Rendering models
        for (auto ref : models.models) {
            Model m = backend->getModel(ref.name);
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

    std::string Component::get(std::string name)
    {
        return values[name];
    }

    void Component::set(std::string name, std::string value)
    {
        values[name] = value;
    }

    void Component::compileAll()
    {
        compile();

        for (auto anchor : anchors) {
            if (anchor->component != NULL) {
                anchor->component->compileAll();
            }
        }
    }

    void Component::compile()
    {
        // Creating CSG 
        std::string csg = module->openscad("csg", parameters());
        // Creating STL and storing it to the model
        myModel = loadModelSTL_string(stl());
        // Parsing the CSG document
        CSG document = CSG::parse(csg);
        anchors = document.anchors;
        parts = document.parts;
        models = document.models;
        bom = document.bom;

        for (auto part : parts.parts) {
            backend->getModule(part.name).openscad("stl", part.parameters);
        }
        for (auto model : models.models) {
            backend->getModule(model.name).openscad("stl", model.parameters);
        }

        int index = 0;
        for (auto anchor : anchors) {
            anchor->component = this;
            anchor->id = index;
            index++;
        }
    }

    void Component::moveAnchors(Component *other)
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

    void Component::detachDiffAnchors(Component *other)
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

    void Component::restore()
    {
        for (auto anchor : anchors) {
            if (anchor->anchor) {
                anchor->anchor->anchor = anchor;
            }
        }
    }

    void Component::detachAll()
    {
        for (auto anchor : anchors) {
            anchor->detach(false);
        }
    }
            
    AnchorPoint *Component::belowAnchor()
    {
        for (auto anchor : anchors) {
            if (anchor->above == false && anchor->anchor) {
                return anchor;
            }
        }

        return NULL;
    }

    AnchorPoint *Component::aboveAnchor()
    {
        AnchorPoint *below = belowAnchor();
        if (below != NULL) {
            return below->anchor;
        }

        return NULL;
    }

    std::string Component::getValue(std::string name)
    {
        if (values.count(name)) {
            return values[name];
        }

        return "";
    }

    Parameters Component::parameters()
    {
        Parameters params;

        for (auto value : values) {
            params.set(value.first, value.second);
        }

        return params;
    }

    std::string Component::stl()
    {
        return module->openscad("stl", parameters(), true);
    }

    Json::Value Component::parametersJson()
    {
        Json::Value json(Json::objectValue);

        for (auto value : values) {
            json[value.first] = value.second;
        }

        return json;
    }

    AnchorPoint *Component::getAnchor(int id)
    {
        for (auto anchor : anchors) {
            if (anchor->id == id) {
                return anchor;
            }
        }

        return NULL;
    }

    void Component::parametersFromJson(Json::Value json)
    {
        for (auto name : json.getMemberNames()) {
            values[name] = json[name].asString();
        }
    }

    Json::Value Component::toJson()
    {
        Json::Value json(Json::objectValue);

        json["component"] = module->getName();
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
                json["anchors"][id]["component"] = anchor->anchor->component->toJson();
            }
        }

        return json;
    }

    void Component::foreachComponent(std::function<void(Component *component)> method)
    {
        method(this);

        for (auto anchor : anchors) {
            if (anchor->anchor != NULL && anchor->above) {
                anchor->anchor->component->foreachComponent(method);
            }
        }
    }

    void Component::foreachAnchor(std::function<void(AnchorPoint *component)> method)
    {
        for (auto anchor : anchors) {
            if (anchor->anchor != NULL && anchor->above) {
                method(anchor);
                anchor->anchor->component->foreachAnchor(method);
            }
        }
    }

    void Component::onHover()
    {
        hover = true;
        for (auto anchor : anchors) {
            anchor->hover = true;
        }
    }
}
