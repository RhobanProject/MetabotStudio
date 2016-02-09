#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <symbolicc++.h>
#include "3d/Point3.h"
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
                std::string id;
                float min, max;
                int jointId;
            };
            class Tip
            {
                public:
                    void addMatrix(TransformMatrix matrix);
                    void addRotation(std::string id, int jointId, float min, float max);
                    Point3 position(std::vector<double> &alphas);
                    std::vector<Point3> positions(const std::vector<double> &alphas);
                    std::vector<ChainItem> chain;
                    std::map<int, int> angles;
                    Symbolic x, y, z;
            };

            Kinematic();
            void display();

            std::string addJoint();
            void addTip(Tip tip);

            std::string code;
            int alphaId;
            int tipId;
            
            std::vector<Tip> tips;
    };
}
