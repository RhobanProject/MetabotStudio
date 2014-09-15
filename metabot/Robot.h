#pragma once

#include <string>
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

            void clear();
            void loadFromFile(std::string filename);
            void saveToFile(std::string filename);

#ifdef OPENGL
            void openGLDraw();
            void unHighlight();
#endif
            ComponentInstance *root;

            Backend *backend;
    };
}
