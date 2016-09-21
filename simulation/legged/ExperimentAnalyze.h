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

class ExperimentAnalyzeStable : public ExperimentController
{
    public:
        void control(Simulation *simulation);
        double scoreGait(std::vector<float> phases);
};

class ExperimentAnalyzeDraw : public ExperimentController
{
    public:
        void control(Simulation *simulation);
};

class ExperimentAnalyzeEffectiveSupport : public ExperimentController
{
    public:
        void init(Simulation *simulation, Parameters &parameters);
        void control(Simulation *simulation);
        double score(Simulation *simulation);

    protected:
        std::vector<Component *> legs;
        std::vector<int> supportFrames;
        int totalFrames;
};
