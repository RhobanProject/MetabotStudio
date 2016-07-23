#pragma once

#include <Experiment.h>
#include <Simulation.h>
#include "ExperimentAnalyze.h"
#include "ExperimentController.h"

class ExperimentAnalyze : public ExperimentController
{
    public:
        void control(Simulation *simulation);
};
