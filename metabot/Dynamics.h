#pragma once

#include "Vector.h"
#include "TransformMatrix.h"

namespace Metabot
{
    class Dynamics
    {
        public:
            Dynamics();

            // Volume (mm3)
            double volume;
            // Center of mass position (mm)
            Vector com;

            Dynamics combine(const Dynamics &other, TransformMatrix matrix);
    };
}
