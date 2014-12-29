#pragma once

#include <string>
#include <map>
#include <vector>
#include <json/json.h>
#include "TransformMatrix.h"
#include "Parameters.h"

namespace Metabot
{
    class Ref
    {
        public:
            Ref(Json::Value json, TransformMatrix matrix);

            /**
             * This hash is used to group the unique parts
             */
            std::string hash();

            int quantity;
            std::string name;
            Parameters parameters;
            float r, g, b;
            TransformMatrix matrix;
    };
    
    class RefsGrouped;
    class Refs : public std::vector<Ref>
    {
        public:
            void add(const Ref &other);
            void merge(const Refs &parts);
            RefsGrouped group();
    };

    class RefsGrouped
    {
        public:
            void add(std::string name, Ref part);
            std::string toString();

            std::map<std::string, Refs> groups;
    };
}
