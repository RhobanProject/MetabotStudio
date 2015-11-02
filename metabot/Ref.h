#pragma once

#include <string>
#include <map>
#include <vector>
#include <json/json.h>
#include <3d/Model.h>
#include "TransformMatrix.h"
#include "Dynamics.h"
#include "Parameters.h"

namespace Metabot
{
    class Backend;
    class Ref
    {
        public:
            Ref(Json::Value json, TransformMatrix matrix, int default_defines=0);

            /**
             * This compiles the part, calling the according method on the
             * backend. 
             * The print flags can be used to add print=true in the arguments
             * of the module.
             */
            void compile(Backend *backend, int defines=0);
            Model doCompile(Backend *backend, int defines=0, bool print=false);
            Model &getModel();

            /**
             * Run the analysis of the part and get its dynamics
             */
            void analyze(Backend *backend);
            Dynamics &getDynamics();
            void setDynamics(Dynamics other);

            /**
             * This hash is used to group the unique parts
             */
            std::string hash();

            int default_defines;
            int quantity;
            std::string name;
            Parameters parameters;
            float r, g, b;
            float density, mass;
            TransformMatrix matrix;

        protected:
            Model model;
            Dynamics dynamics;
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

    class RefsGrouped : public std::map<std::string, std::vector<Ref>>
    {
        public:
            void add(std::string name, Ref part);
            std::string toString();
    };
}
