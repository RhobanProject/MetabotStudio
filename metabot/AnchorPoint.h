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
            AnchorPoint(std::string type, bool male, bool female, TransformMatrix matrix, float zero);
            virtual ~AnchorPoint();
            AnchorPoint *clone();
            void copyData(AnchorPoint *other);

            void revert();
            
            bool isCompatible(AnchorPoint *anchor);
            void attach(AnchorPoint *anchor, bool above=true);
            void detach(bool remove=true);
            
            TransformMatrix transformationForward();
            TransformMatrix transformationBackward();

            Model toModel();
            bool highlight;
            bool hover;

#ifdef OPENGL
            void openGLDraw(int id=1);
#endif

            std::string type;
            std::string model;
            TransformMatrix matrix;

            // Matrixes caches
            TransformMatrix forwardAbove, backwardAbove;
            TransformMatrix forward, backward;
            bool cached;
            void computeMatrixes();

            // Anchor id
            int id;

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
            float zero;

            // Orientation
            float orientationX, orientationY, orientationZ;
    };
}
