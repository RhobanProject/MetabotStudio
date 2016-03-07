#pragma once

#include "Experience.h"

class ExperienceWalk : public Experience
{
    public:
        virtual void control(Simulation *simulation);
};
