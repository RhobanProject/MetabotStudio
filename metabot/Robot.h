#pragma once

#include <string>
#include <functional>
#include <3d/Model.h>
#include <3d/Point3.h>
#include "Vector.h"
#ifndef NOCPP11
#include <functional>
#endif

namespace Metabot
{
    class Backend;
    class AnchorPoint;
    class ComponentInstance;
    class Robot
    {
        public:
            Robot(Backend *backend);
            virtual ~Robot();
            void compile();
            Model toModel();

            void build(std::string directory);

            Robot *clone();
            void setRoot(ComponentInstance *root);
            Vector getPoint(ComponentInstance *instance, Vector pt);
            ComponentInstance *nearest(Vector pt);
            ComponentInstance *getComponentById(int id);

            void clear();
            void loadFromFile(std::string filename);
            void saveToFile(std::string filename);
            void number();

#ifndef NOCPP11
            void foreach(std::function<void(ComponentInstance *instance)> method);
            void foreachAnchor(std::function<void(AnchorPoint *anchor)> method);
#endif
            std::vector<float> getZeros();
            std::vector<AnchorPoint*> getAnchors();
            void setZeros(std::vector<float> zeros);
            void highlightNth(int nth);

#ifdef OPENGL
            void openGLDraw();
#endif
            void unHighlight();
            ComponentInstance *root;

            Backend *backend;
    };
}
