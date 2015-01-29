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
#include "Backend.h"
#include "Cache.h"
#include "CSG.h"
#include "Robot.h"
#include "util.h"

namespace Metabot
{
    Component::Component(Backend *backend_, Module *module_)
        : backend(backend_), robot(NULL), module(module_), highlight(false), 
        hover(false), main(Json::Value(), TransformMatrix::identity(), true)
    {
        for (auto param : module->getParameters()) {
            values[param.second.name] = param.second.getValue();
        }
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

        component->shapes = shapes;
        component->collisions = collisions;
        component->dynamics = dynamics;
        component->main = main;
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
            
    Dynamics Component::getDynamics()
    {
        return dynamics;
    }
            
    void Component::computeDynamics()
    {
        dynamics = Dynamics();

        for (auto ref : refs()) {
            dynamics.combine(ref->getDynamics(), ref->matrix);
        }
    }
            
    void Component::walkDynamics(Dynamics &global, TransformMatrix matrix, bool verbose)
    {
        Dynamics my_dynamics = getDynamics();
        if (verbose) {
            std::cout << "* Combining component " << module->getName() << std::endl;
            std::cout << my_dynamics.toString() << std::endl;
        }
        global.combine(my_dynamics, matrix);

        for (auto anchor : anchors) {
            if (anchor->above && anchor->anchor) {
                auto m = matrix.multiply(anchor->transformationForward());
                m = m.multiply(anchor->anchor->transformationBackward());
                anchor->anchor->component->walkDynamics(global, m, verbose);
            }
        }
    }
            
    void Component::writeURDF(std::stringstream &ss, std::string parent, TransformMatrix parentPreTransform, AnchorPoint *above)
    {
        Dynamics dynamics;

        ss << std::endl << "  <!-- Component " << module->getName();
        ss << "#" << id << " -->" << std::endl << std::endl;

        // Adding the component and linking it to the parent
        std::stringstream tmp;
        tmp << module->getName() << "_" << id;
        std::string name = tmp.str();
        ss << "  <link name=\"" << name << "\">" << std::endl;

        // Adding parts and models, linked to component
        int refid = 0;
        auto preTransform = TransformMatrix::identity();
        if (above != NULL) {
            preTransform = above->transformationBackward();
        }
        for (auto ref : refs()) {
            dynamics.combine(ref->getDynamics(), ref->matrix);
            tmp.str("");
            tmp << module->getName() << "_" << ref->name << "_" << id << "_" << (refid++);
            auto refName = tmp.str();
            auto jointName = refName+"_joint";

            ss << "    <visual>" << std::endl;
            ss << "      <geometry>" << std::endl;
            // XXX: This forces the file to be in an urdf/ folder
            ss << "        <mesh filename=\"package://urdf/" << ref->hash() << ".stl\"/>" << std::endl;
            ss << "      </geometry>" << std::endl;
            ss << "      <material name=\"" << refName << "_material\">" << std::endl;
            ss << "        <color rgba=\"" << ref->r << " " << ref->g << " " << ref->b << " 1.0\"/>" << std::endl;
            ss << "      </material>" << std::endl;
            ss << "    " << preTransform.multiply(ref->matrix).toURDF() << std::endl;
            ss << "    </visual>" << std::endl;
        }

        // Adding dynamics
        ss << "  <inertial>" << std::endl;
        auto com = preTransform.apply(dynamics.com);;
        ss << "    <origin xyz=\"" << (com.x()/1000) << " " 
            << (com.y()/1000) << " " << (com.z()/1000) << "\" rpy=\"0 0 0\"/>" << std::endl;
        // XXX: Todo, handle density
        ss << "    <mass value=\"" << (dynamics.volume*1.2/1000000.0) << "\"/>" << std::endl;
        // XXX: Todo, handle tensor
        ss << "    <inertia ixx=\"0.001\"  ixy=\"0\"  ixz=\"0\" iyy=\"0.001\" iyz=\"0\" izz=\"0.001\" />" << std::endl;
        ss << "  </inertial>" << std::endl;

        // Adding collisions
        for (auto shape : shapes) {
            ss << "  <collision>" << std::endl;
            ss << shape.toURDF(preTransform) << std::endl;
            ss << "  </collision>" << std::endl;
        }
            
        ss << "  </link>" << std::endl;

        
        // Linking it to the parent
        if (above!=NULL) {
            ss << "  <joint name=\"" << name << "_parent\" type=\"revolute\">" << std::endl;
            ss << "    <parent link=\"" << parent << "\"/>" << std::endl;
            ss << "    <child link=\"" << name << "\"/>" << std::endl;
            ss << "    <axis xyz=\"0 0 1\"/>" << std::endl;
            ss << parentPreTransform.multiply(above->anchor->transformationForward()).toURDF() << std::endl;
            ss << "    <limit lower=\"" << -M_PI << "\" upper=\"" << M_PI << "\"/>" << std::endl;
            ss << "  </joint>" << std::endl;
        }

        // Drawing sub-components
        for (auto anchor : anchors) {
            if (anchor->above && anchor->anchor!=NULL) {
                anchor->anchor->component->writeURDF(ss, name, preTransform, anchor->anchor);
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
    void Component::openGLDraw(bool drawCollisions)
    {
        glStencilFunc(GL_ALWAYS, id, -1);

        if (drawCollisions) {
            if (highlight) {
                collisions.r = 0.4;
                collisions.g = 1.0;
                collisions.b = 0.3;
            } else {
                collisions.r = collisions.g = collisions.b = 0.6;
            }
            collisions.openGLDraw();
        }

        // Rendering models & parts
        if (!drawCollisions) {
            for (auto ref : refs()) {
                openGLDrawRef(ref);
            }
        }

        // Rendering sub-components
        int anchorId = 1;
        for (auto anchor : anchors) {
            glPushMatrix();
            if (anchor->above) {
                anchor->openGLDraw(anchorId, drawCollisions);
            }
            glPopMatrix();
            anchorId++;
        }
    }

    void Component::openGLDrawRef(Ref *ref)
    {
        glPushMatrix();
        ref->matrix.openGLMult();
        auto model = ref->getModel();
        if (highlight) {
            model.r = 0.4;
            model.g = 1.0;
            model.b = 0.3;
        } else {
            model.r = ref->r;
            model.g = ref->g;
            model.b = ref->b;
        }
        model.openGLDraw();
        glPopMatrix();
    }
#endif

    Model Component::toModel()
    {
        Model model;

        // Rendering models
        for (auto ref : refs()) {
            Model m = ref->getModel();
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

    std::string Component::get(std::string name)
    {
        return values[name];
    }

    void Component::set(std::string name, std::string value)
    {
        values[name] = value;
    }

    void Component::compileAll(Robot *robot)
    {
        compile(robot);

        for (auto anchor : anchors) {
            if (anchor->component != NULL) {
                anchor->component->compileAll(robot);
            }
        }
    }

    void Component::compile(Robot *robot)
    {
        // Creating CSG 
        std::string csg = module->openscad("csg", parameters(robot));
        // Main reference
        main.name = module->getName();
        main.parameters.clear();
        for (auto entry : values) {
            main.parameters.set(entry.first, entry.second);
        }
        // Parsing the CSG document
        CSG document = CSG::parse(csg);
        anchors = document.anchors;
        parts = document.parts;
        models = document.models;
        bom = document.bom;
        
        // Collision CSG & STL
        std::string collisionsCsg = module->openscad("csg", parameters(robot), false, true);
        collisions = loadModelSTL_string(stl(robot, true));
        CSG collisionsDocument = CSG::parse(collisionsCsg);
        shapes = collisionsDocument.shapes;

        main.compile(backend);
        for (auto &ref : parts) {
            ref.compile(backend);
        }
        for (auto &ref : models) {
            ref.compile(backend);
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

    Parameters Component::parameters(Robot *robot)
    {
        Parameters params = module->getParameters();

        for (auto entry : values) {
            auto key = entry.first;
            auto value = entry.second;
            if (value.size()>0 && value[0]=='$' && robot!=NULL) {
                params.update(key, robot->getValue(value.substr(1)));
            } else {
                params.update(key, value);
            }
        }

        return params;
    }

    std::string Component::stl(Robot *robot, bool drawCollisions)
    {
        return module->openscad("stl", parameters(robot), !drawCollisions, drawCollisions);
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
        values = Values::fromJson(json);
    }

    Json::Value Component::toJson()
    {
        Json::Value json(Json::objectValue);

        json["component"] = module->getName();
        json["parameters"] = values.toJson();
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

    std::vector<Ref*> Component::refs()
    {
        std::vector<Ref*> refs;

        refs.push_back(&main);
        for (auto &ref : models) {
            refs.push_back(&ref);
        }
        for (auto &ref : parts) {
            refs.push_back(&ref);
        }

        return refs;
    }
}
