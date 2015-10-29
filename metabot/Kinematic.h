#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <symbolicc++.h>

namespace Metabot
{
    class Kinematic
    {
        public:
            Kinematic();
            void display();

            std::string addJoint();
            void addTip(Symbolic x, Symbolic y, Symbolic z, std::vector<Symbolic> dependances);

            std::string code;
            int alpha;
            int tip;
            
            struct Tip { 
                std::vector<Symbolic> dependances;
                Symbolic x, y, z;
            };
            std::vector<Tip> tips;
    };
}
