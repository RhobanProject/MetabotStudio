#pragma once

#include <string>
#include <functional>
#include <3d/Model.h>
#include <3d/Point3.h>
#include "Ref.h"
#include "Vector.h"
#include "BOM.h"

namespace Metabot
{
    class Backend;
    class AnchorPoint;
    class Component;
    class Robot
    {
        public:
            Robot(Backend *backend);
            virtual ~Robot();
            void compile();
            Model toModel();

            void build(std::string directory);

            RefsGrouped writeSTLs(std::string directory);
            void writeURDF(std::string directory);

            void computeDynamics();
            void printDynamics();

            Robot *clone();
            void setRoot(Component *root);
            Vector getPoint(Component *instance, Vector pt);
            Component *nearest(Vector pt);

            Component *getComponentById(int id);
            AnchorPoint *getHoveredAnchor(int id);

            void clear();
            void loadFromFile(std::string filename);
            void saveToFile(std::string filename);
            void number();

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
            Component *root;

            bool drawCollisions;
            void setDrawCollisions(bool draw=false);

            Backend *backend;

            Refs getParts();
            BOM getBOM();
    };
}
