#pragma once

#include <Robot.h>
#include "Controller.h"
#include <Experiment.h>

class ExperimentController : public Experiment
{
    public:
        virtual ~ExperimentController();
        void init(Simulation *simulation, Parameters &parameters);
        void control(Simulation *simulation);
        double collisionsPenalty();
        
        static void initParameters(Parameters &parameters, Metabot::Robot *robot);

    protected:
        Controller *controller;

        double cost;
        double collisions;
};
