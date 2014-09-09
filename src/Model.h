#pragma once

#include <string>
#include "TransformMatrix.h"

namespace Metabot
{
    class Model
    {
        public:
            Model(std::string name, TransformMatrix matrix);

            std::string name;
            TransformMatrix matrix;
    };
}
