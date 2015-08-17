#pragma once

#include "Vect.h"
#include "TransformMatrix.h"

namespace Metabot
{
    class Dynamics
    {
        public:
            Dynamics();

            // Volume (mm3)
            double volume;
            // Mass (g)
            double mass;
            // Center of mass position (mm)
            Vect com;
            // Is this dynamics computed?
            bool computed;

            std::string toString();

            void combine(const Dynamics &other, TransformMatrix matrix);
    };
}
