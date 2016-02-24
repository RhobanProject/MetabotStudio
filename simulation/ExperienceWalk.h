#pragma once

#include "ExperienceController.h"

class ExperienceWalkEfficience : public ExperienceController
{
    public:
        double score(Simulation *simulation);
};

class ExperienceWalkSpeed : public ExperienceController
{
    public:
        double score(Simulation *simulation);
};
