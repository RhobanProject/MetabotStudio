#pragma once

#include <vector>
#include <3d/Model.h>

namespace Metabot
{
    class Voxels
    {
        public:
            Voxels(int width, int height, int depth);

            void static voxelize(Model m);

        protected:
            std::vector<bool> points;
    };
}
