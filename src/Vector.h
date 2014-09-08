#pragma once

#include <string>

namespace Metabot
{
    class Vector
    {
        public:
            Vector(float x, float y, float z);
            Vector();

            float values[4];

            std::string toString();
    };
}
