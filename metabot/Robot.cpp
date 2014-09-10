#include <iostream>
#include "Robot.h"
#include "ComponentInstance.h"

namespace Metabot
{
    Robot::Robot()
        : root(NULL)
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
        root->compileAll();
    }
    
    void Robot::computeModel()
    {
        root->computeModel();
    }
}
