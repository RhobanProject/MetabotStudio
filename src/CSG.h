#pragma once

#include <string>

namespace Metabot
{
    class CSGNode;
    class CSG
    {
        public:
            CSG();
            virtual ~CSG();
            static CSG *parse(std::string data);

            CSGNode *root;
    };
}
