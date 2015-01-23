#pragma once

#include <string>

namespace Metabot
{
    class Vector
    {
        public:
            Vector(float x, float y, float z);
            Vector();

            float distance(const Vector &other);
            float values[4];
            float x();
            float y();
            float z();

            std::string toString();
    };
}
