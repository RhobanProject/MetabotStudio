#pragma once

#include "Experience.h"

class ExperienceZero : public Experience
{
    public:
        virtual void control(Simulation *simulation);
};
