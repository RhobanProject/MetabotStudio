#pragma once

#ifndef NOCPP11
#include <functional>
#endif
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <3d/Model.h>
#include <json/json.h>
#include "BOM.h"
#include "Ref.h"
#include "Shape.h"
#include "Parameters.h"
#include "Dynamics.h"

namespace Metabot
{
    class Backend;
    class AnchorPoint;
    class Module;

    class Component
    {
        public:
            Component(Backend *backend, Module *module);
            virtual ~Component();
            Component *clone();

            void root();

            void computeDynamics();
            Dynamics getDynamics();
            void walkDynamics(Dynamics &global, TransformMatrix matrix=TransformMatrix::identity(), bool verbose=false);
            void writeURDF(std::stringstream &ss, std::string parent="", 
                    TransformMatrix preTransform=TransformMatrix::identity(), AnchorPoint *above=NULL);

            Model toModel();
            AnchorPoint *findCompatible(AnchorPoint *anchor);
            bool isCompatible(AnchorPoint *anchor);

            std::string fullName();

            Json::Value parametersJson();
            void parametersFromJson(Json::Value json);
            Json::Value toJson();

            void compileAll();
            void compile();

#ifndef NOCPP11
            void foreachComponent(std::function<void(Component *instance)> method);
            void foreachAnchor(std::function<void(AnchorPoint *instance)> method);
#endif
            
#ifdef OPENGL
            void openGLDraw(bool drawCollisions=false);
            void openGLDrawRef(Ref *ref);
#endif
            bool highlight;
            bool hover;
            void onHover();
    
            // Merge the anchors from another entity
            // If detach is true, anchor will be detached from the other objects, and thus
            // "taken" from it
            void moveAnchors(Component *other);
            void detachDiffAnchors(Component *other);
            void restore();
            void detachAll();

            // Gets the point that is attached with the robot upper
            AnchorPoint *belowAnchor();
            AnchorPoint *aboveAnchor();

            std::string getValue(std::string name);
            std::string stl(bool drawCollisions=false);
            Parameters parameters();

            // Backend and module
            Backend *backend;
            Module *module;

            // Accessing parameter values
            std::string get(std::string name);
            void set(std::string name, std::string value);
            
            AnchorPoint *getAnchor(int id);

            std::map<std::string, std::string> values;
            Component *component;

            std::vector<Ref*> refs();
 
            std::vector<AnchorPoint *> anchors;
            Ref main;
            Refs models;
            Refs parts;
            BOM bom;
            std::vector<Shape> shapes;

            Model collisions;
            Dynamics dynamics;

            int id;
    };
}
