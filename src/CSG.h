#pragma once

#include <vector>
#include <string>
#include "TransformMatrix.h"
#include "AnchorPoint.h"

namespace Metabot
{
    class CSGNode;
    class CSG
    {
        public:
            CSG();
            virtual ~CSG();
            static CSG *parse(std::string data);

            void walk(TransformMatrix matrix, CSGNode *node);

            std::vector<AnchorPoint*> anchors;
            std::vector<std::string> parts;

            CSGNode *root;
    };
}
