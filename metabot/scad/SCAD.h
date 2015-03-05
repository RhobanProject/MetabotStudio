#pragma once

#include <map>
#include <vector>
#include <string>
#include "Module.h"

namespace Metabot
{
    class Backend;
    class SCAD
    {
        public:
            SCAD(std::string filename);

            void process();

            std::vector<Module> getModules();

            /**
             * Reading a .scad file, this will generate the associated
             * .metabot.scad file and generate the list of all modules 
             * found.
             */
            static std::vector<Module> load(std::string filename);

            /**
             * Generating a metabot marker from json
             */
            static std::string jsonMarker(Json::Value json);

        protected:
            std::string filename, outputFilename;
            // File output
            std::ostringstream oss;
            // Parser state
            int state;
            // Module parsing variables
            int moduleState;
            std::string tmpName, tmpValue;
            int brackets, equals;
            // Current module
            Module module;
            // Obtained modules
            std::vector<Module> modules;
            
            void processLine(std::string line);
            void processModuleLine(std::string line);
    };
}
