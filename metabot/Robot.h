#pragma once

#include <functional>
#include <3d/Model.h>

namespace Metabot
{
    class Backend;
    class ComponentInstance;
    class Robot
    {
        public:
            Robot(Backend *backend);
            void compile();
            Model toModel();

#ifdef OPENGL
            void openGLDraw();
            void shadow();
            void unShadow();
#endif
            ComponentInstance *root;

            Backend *backend;
    };
}
