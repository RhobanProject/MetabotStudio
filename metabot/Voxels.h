#pragma once

#include <vector>
#include <3d/Model.h>

#include "Dynamics.h"

namespace Metabot
{
    class Voxels
    {
        public:
            Voxels();
            Dynamics static voxelize(Model m);
    };
}
