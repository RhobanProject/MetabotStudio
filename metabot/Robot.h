#pragma once

#include <string>
#include <functional>
#include <3d/Model.h>
#include <3d/Point3.h>
#include "Vector.h"
#include "BOM.h"

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
            AnchorPoint *getHoveredAnchor(int id);

            void clear();
            void loadFromFile(std::string filename);
            void saveToFile(std::string filename);
            void number();

            void foreach(std::function<void(ComponentInstance *instance)> method);
            void foreachAnchor(std::function<void(AnchorPoint *anchor)> method);
            std::vector<float> getZeros();
            std::vector<AnchorPoint*> getAnchors();
            void setZeros(std::vector<float> zeros);

#ifdef OPENGL
            void openGLDraw();
#endif
            void unHighlight();
            void unHover();
            ComponentInstance *root;

            Backend *backend;
            
            BOM getBOM();
    };
}
