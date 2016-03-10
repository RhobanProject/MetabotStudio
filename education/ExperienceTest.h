#pragma once

#include "Experience.h"

class ExperienceZero : public Experience
{
    public:
        virtual void control(Simulation *simulation);
};

class ExperienceSinus : public Experience
{
    public:
        virtual void control(Simulation *simulation);
};
