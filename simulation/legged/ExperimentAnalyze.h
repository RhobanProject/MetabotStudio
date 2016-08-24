#pragma once

#include <Experiment.h>
#include <Simulation.h>
#include "ExperimentAnalyze.h"
#include "ExperimentController.h"

class ExperimentAnalyzeDump : public ExperimentController
{
    public:
        void control(Simulation *simulation);
};

class ExperimentAnalyzeInitial : public ExperimentController
{
    public:
        void control(Simulation *simulation);
};

class ExperimentAnalyzePath : public ExperimentController
{
    public:
        ExperimentAnalyzePath();
        void control(Simulation *simulation);
        double score(Simulation *simulation);
    
    protected:
        Vect cop;
};
