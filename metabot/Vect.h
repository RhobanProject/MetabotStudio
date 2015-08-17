#pragma once

#include <string>

namespace Metabot
{
    class Vect
    {
        public:
            Vect(float x, float y, float z);
            Vect();

            float distance(const Vect &other);
            float values[4];
            float x() const;
            float y() const;
            float z() const;

            std::string toString() const;
    };
}
