#pragma once

#include <string>

namespace Metabot
{
    class Part
    {
        public:
            Part(std::string name, std::string params);
            Part *clone();

            std::string name, params;
    };
}
