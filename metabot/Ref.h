#pragma once

#include <string>
#include <map>
#include <vector>
#include <json/json.h>
#include <3d/Model.h>
#include "TransformMatrix.h"
#include "Parameters.h"

namespace Metabot
{
    class Backend;
    class Ref
    {
        public:
            Ref(Json::Value json, TransformMatrix matrix);

            void compile(Backend *backend);
            Model &getModel();

            /**
             * This hash is used to group the unique parts
             */
            std::string hash();

            int quantity;
            std::string name;
            Parameters parameters;
            float r, g, b;
            TransformMatrix matrix;

        protected:
            Model model;
            Backend *backend;
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
