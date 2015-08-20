#pragma once

#include <vector>
#include <string>
#include "TransformMatrix.h"
#include "AnchorPoint.h"
#include "Ref.h"
#include "BOM.h"
#include "Shape.h"

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
            std::vector<Shape> shapes;
            std::vector<TransformMatrix> contacts;
            Refs models;
            Refs parts;
            BOM bom;

            CSGNode *root;
    };
}
