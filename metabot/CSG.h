#pragma once

#include <vector>
#include <string>
#include "TransformMatrix.h"
#include "AnchorPoint.h"
#include "Part.h"
#include "ModelRef.h"
#include "BOM.h"

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
            std::vector<Part*> parts;
            std::vector<ModelRef*> models;
            BOM bom;

            CSGNode *root;
    };
}
