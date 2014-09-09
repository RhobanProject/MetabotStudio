#pragma once

#include <string>
#include <3d/Model.h>
#include "TransformMatrix.h"
#include "ComponentInstance.h"

namespace Metabot
{
    class Component;
    class ModelRef
    {
        public:
            ModelRef(std::string name, TransformMatrix matrix);

            std::string name;
            TransformMatrix matrix;
    };
}
