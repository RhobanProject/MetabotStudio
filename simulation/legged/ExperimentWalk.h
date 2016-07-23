#pragma once

#include "ExperimentController.h"

class ExperimentWalkEfficience : public ExperimentController
{
    public:
        double score(Simulation *simulation);
};

class ExperimentWalkSpeed : public ExperimentController
{
    public:
        double score(Simulation *simulation);
};
