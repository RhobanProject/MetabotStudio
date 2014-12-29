#pragma once

#include <vector>
#include <string>
#include <3d/Model.h>
#include <json/json.h>
#include "Parameters.h"
#include "TransformMatrix.h"

namespace Metabot
{
    class ModelRef
    {
        public:
            ModelRef(Json::Value json, TransformMatrix matrix);
            ModelRef(std::string name_, float r, float g, float b, TransformMatrix matrix);

            std::string name;
            TransformMatrix matrix;
            float r, g, b;
            Parameters parameters;
    };

    class ModelRefs
    {
        public:
            void add(const ModelRef &model);

            std::vector<ModelRef> models;
    };
}
