#pragma once

#include <string>
#include <3d/Model.h>
#include <json/json.h>
#include "TransformMatrix.h"
#include "ComponentInstance.h"

namespace Metabot
{
    class Component;
    class ModelRef
    {
        public:
            ModelRef(Json::Value data, TransformMatrix matrix);

            std::string name;
            TransformMatrix matrix;

            float r, g, b;
    };
}
