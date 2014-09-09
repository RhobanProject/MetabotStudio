#pragma once

#include <string>
#include "TransformMatrix.h"

namespace Metabot
{
    class ModelRef
    {
        public:
            ModelRef(std::string name, TransformMatrix matrix);

            std::string name;
            TransformMatrix matrix;
    };
}
