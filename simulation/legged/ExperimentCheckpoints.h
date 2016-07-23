#pragma once

#include "ExperimentController.h"

class ExperimentCheckpoints : public ExperimentController
{
    public:
        void init(Simulation *simulation, Parameters &parameters);
        virtual void control(Simulation *simulation);
        virtual double score(Simulation *simulation);
        virtual double scoreOver(Simulation *simulation)=0;

        static void initParameters(Parameters &parameters, Metabot::Robot *robot);
        static double defaultDuration();
        
        virtual bool optimizeTurn();

    protected:
        double controlT;
        int announcedCheckpoint;
        int currentCheckpoint;
        std::vector<FPoint2> checkpoints;
        double maxStep, maxTurn, kT;
};

class ExperimentCheckpointsEfficience : public ExperimentCheckpoints
{
    public:
        double scoreOver(Simulation *simulation);
};

class ExperimentCheckpointsSpeed : public ExperimentCheckpoints
{
    public:
        double scoreOver(Simulation *simulation);
};
