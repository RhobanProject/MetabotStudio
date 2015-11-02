#pragma once

#include <vector>
#include "Vect.h"
#include "TransformMatrix.h"

namespace Metabot
{
    struct DynamicsCube
    {
        Vect pos;
        float mass;
    };

    class Dynamics
    {
        public:
            Dynamics();

            // Volume (mm3)
            double volume;
            // Mass (g)
            double mass;
            // Center of mass position (m)
            Vect com;
            // Is this dynamics computed?
            bool computed;
            // Inertia
            float ixx, ixy, ixz, iyy, iyz, izz;
            // Cubes
            std::vector<DynamicsCube> cubes;
            
            // Update inertia from cubes
            void updateInertia();

            std::string toString();

            void combine(const Dynamics &other, TransformMatrix matrix);

            std::string serialize();
            static Dynamics unserialize(std::string data);
    };
}
