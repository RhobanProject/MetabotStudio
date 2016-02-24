#pragma once

#include "ExperienceController.h"

class ExperienceCheckpoints : public ExperienceController
{
    public:
        virtual void init(Experience::Parameters &parameters, Metabot::Robot *robot);
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
        double maxStep, maxTurn;
};

class ExperienceCheckpointsEfficience : public ExperienceCheckpoints
{
    public:
        double scoreOver(Simulation *simulation);
};

class ExperienceCheckpointsSpeed : public ExperienceCheckpoints
{
    public:
        double scoreOver(Simulation *simulation);
};
