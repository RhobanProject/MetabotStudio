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

            bool is(std::string type);

            std::string fix(std::string data);
            void process(std::string data);

            Json::Value json;
            std::string name, value;
            std::vector<CSGNode*> children;
    };
}
