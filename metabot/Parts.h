#pragma once

#include <string>
#include <vector>
#include <map>
#include <json/json.h>
#include "Parameters.h"
#include "TransformMatrix.h"

namespace Metabot
{
    class Part;
    class Parts;
    class PartsGrouped;

    class Part
    {
        public:
            Part(Json::Value json, TransformMatrix matrix);

            std::string hash();

            int quantity;
            std::string name;
            Parameters parameters;
            TransformMatrix matrix;
    };

    class Parts
    {
        public:
            void add(const Part &other);
            void merge(const Parts &parts);
            PartsGrouped group();

            std::vector<Part> parts;
    };

    class PartsGrouped
    {
        public:
            void add(std::string name, Part part);
            std::string toString();

            std::map<std::string, Parts> groups;
    };
}
