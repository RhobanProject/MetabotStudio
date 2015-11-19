#pragma once

#include <string>
#include <functional>
#include <3d/Model.h>
#include <3d/Point3.h>
#include "Ref.h"
#include "Vect.h"
#include "Dynamics.h"
#include "scad/BOM.h"
#include "Values.h"
#include "Kinematic.h"
#include "bullet/World.h"

namespace Metabot
{
    class Backend;
    class AnchorPoint;
    class Component;
    class Robot
    {
        public:
            Robot(Backend *backend=NULL);
            virtual ~Robot();
            void compile();
            Model toModel();

            void build(std::string directory);

            RefsGrouped writeSTLs(std::string directory);
            void writeJS(std::string directory);
            void writeSDF(std::string directory);

            void computeDynamics();
            Dynamics getDynamics();
            void printDynamics();
            Kinematic computeKinematic();

            Robot *clone();
            void setRoot(Component *root);
            Vect getPoint(Component *instance, Vect pt);
            Component *nearest(Vect pt);

            Component *getComponentById(int id);
            AnchorPoint *getHoveredAnchor(int id);

            void clear();
            void fromJson(Json::Value json, Values values = Values());
            void loadFromFile(std::string filename, Values values = Values());
            Json::Value toJson();
            void saveToFile(std::string filename);
            void number();

            Json::Value stateToJson();
            void stateFromJson(Json::Value json);

            void foreachComponent(std::function<void(Component *instance)> method);
            void foreachAnchor(std::function<void(AnchorPoint *anchor)> method);
            std::vector<float> getZeros();
            std::vector<AnchorPoint*> getAnchors();
            void setZeros(std::vector<float> zeros);

#ifdef OPENGL
            void openGLDraw();
#endif
            void unHighlight();
            void unHover();

            std::string getValue(std::string name);

            void toBullet();
            TransformMatrix getState();

            Component *root;
            Values parameters;

            bool drawCollisions;
            void setDrawCollisions(bool draw=false);
            bool drawCOM;
            void setDrawCOM(bool draw=false);

            Backend *backend;

            Refs getParts();
            BOM getBOM();

            World world;
    };
}
