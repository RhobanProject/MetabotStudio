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
            /**
             * Reading a .scad file, this will generate the associated
             * .metabot.scad file and generate the list of all modules 
             * found.
             */
            static std::vector<Module> load(std::string filename);
    };
}
