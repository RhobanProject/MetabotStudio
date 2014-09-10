#pragma once

#include <string>

namespace Metabot
{
    class Part
    {
        public:
            Part(std::string name, std::string params);

            std::string name, params;
    };
}
