#pragma once

#include <string>
#include <3d/Model.h>
#include <json/json.h>
#include "TransformMatrix.h"
#include "ComponentInstance.h"

namespace Metabot
{
    class Component;
    class AnchorPoint
    {
        public:
            AnchorPoint(Json::Value json, TransformMatrix matrix);
            virtual ~AnchorPoint();
            
            bool isCompatible(AnchorPoint *anchor);
            void attach(AnchorPoint *anchor, bool above=true);
            void detach(bool remove=true);

            Model toModel();
#ifdef OPENGL
            void openGLDraw();
#endif

            std::string type;
            std::string model;
            TransformMatrix matrix;

            // Anchor type
            bool female, male;

            // Component instance related to this anchor
            ComponentInstance *instance;

            // Other anchor which is connected to this
            AnchorPoint *anchor;

            // Is this anchor below the other in the tree?
            bool above;

            // Rotate angle
            float alpha;
    };
}
