#include <iostream>
#include "Robot.h"
#include "ComponentInstance.h"

namespace Metabot
{
    Robot::Robot(Backend *backend_)
        : root(NULL), backend(backend_)
    {
    }

    Model Robot::toModel()
    {
        if (root != NULL) {
            return root->toModel();
        } else {
            Model model;
            return model;
        }
    }

    void Robot::compile()
    {
        if (root != NULL) {
            root->compileAll();
        }
    }
            
    void Robot::openGLDraw()
    {
        if (root != NULL) {
            root->openGLDraw();
        }
    }
            
#ifdef OPENGL
    void Robot::shadow()
    {
        if (root != NULL) {
            root->setBrightness(0.4, true);
        }
    }

    void Robot::unShadow()
    {
        if (root != NULL) {
            root->setBrightness(1.0, true);
        }
    }
#endif
}
