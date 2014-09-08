#pragma once

#include <vector>
#include <string>
#include "TransformMatrix.h"

namespace Metabot
{
    class CSGNode
    {
        public:
            CSGNode(std::string name, std::string value="");
            virtual ~CSGNode();

            // For multmatrix nodes
            bool isMatrix();
            TransformMatrix matrix;

            // For marker nodes
            std::string data;
            bool isMarker();
            bool isAnchor();
            bool isModel();
            bool isPart();

            std::string name, value;
            std::vector<CSGNode*> children;
            bool anchor, model, part;
    };
}
