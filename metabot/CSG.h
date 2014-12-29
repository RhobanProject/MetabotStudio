#pragma once

#include <vector>
#include <string>
#include "TransformMatrix.h"
#include "AnchorPoint.h"
#include "Parts.h"
#include "ModelRefs.h"
#include "BOM.h"

namespace Metabot
{
    class CSGNode;
    class CSG
    {
        public:
            CSG();
            virtual ~CSG();
            static CSG parse(std::string data);

            void walk(TransformMatrix matrix, CSGNode *node);

            std::vector<AnchorPoint*> anchors;
            ModelRefs models;
            Parts parts;
            BOM bom;

            CSGNode *root;
    };
}
