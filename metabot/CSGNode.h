#pragma once

#include <vector>
#include <string>
#include <json/json.h>
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

            Json::Value json();

            std::string name, value;
            std::vector<CSGNode*> children;
            std::vector<std::string> parameters;
            bool anchor, model, part, parameter, bom;
    };
}
