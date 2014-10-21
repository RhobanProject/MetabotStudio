#pragma once

#include <string>
#include "TransformMatrix.h"

namespace Metabot
{
    class Part
    {
        public:
            Part(std::string name, std::string params, TransformMatrix matrix);
            Part *clone();

            std::string hash();

            int quantity;
            std::string name, params;
            TransformMatrix matrix;
    };
}
