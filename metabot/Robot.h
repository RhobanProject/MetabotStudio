#pragma once

#include <3d/Model.h>

namespace Metabot
{
    class ComponentInstance;
    class Robot
    {
        public:
            Robot();
            void compile();
            void computeModel();
            Model toModel();

            ComponentInstance *root;
    };
}
