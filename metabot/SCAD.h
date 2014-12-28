#pragma once

#include <map>
#include <vector>
#include <string>
#include "ComponentParameter.h"
#include "Module.h"

namespace Metabot
{
    class Backend;
    class SCAD
    {
        public:
            static std::vector<Module> load(std::string filename);
    };
}
