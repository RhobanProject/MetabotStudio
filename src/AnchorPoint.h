#pragma once

#include <string>
#include "TransformMatrix.h"
#include "ComponentInstance.h"

namespace Metabot
{
    class Component;
    class AnchorPoint
    {
        public:
            AnchorPoint(std::string type, TransformMatrix matrix);

            std::string type;
            TransformMatrix matrix;
            ComponentInstance *instance;
    };
}
