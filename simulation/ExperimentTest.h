#pragma once

#include "Experiment.h"

class ExperimentZero : public Experiment
{
    public:
        virtual void control(Simulation *simulation);
};

class ExperimentSinus : public Experiment
{
    public:
        virtual void control(Simulation *simulation);
};
