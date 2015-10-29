#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <symbolicc++.h>
#include "TransformMatrix.h"

#define CHAIN_MATRIX    0
#define CHAIN_ROTATION  1

namespace Metabot
{
    class Kinematic
    {
        public:
            struct ChainItem
            {
                int type;
                TransformMatrix matrix;
                int alpha;
            };
            class Chain
            {
                public:
                    void addMatrix(TransformMatrix matrix);
                    void addRotation(int alpha);
                    std::vector<ChainItem> items;
            };

            Kinematic();
            void display();

            std::string addJoint();
            void addTip(Symbolic x, Symbolic y, Symbolic z, Chain chain);

            std::string code;
            int alpha;
            int tip;
            
            struct Tip { 
                Symbolic x, y, z;
                Chain chain;
            };
            std::vector<Tip> tips;
    };
}
