#pragma once

#include <map>
#include <string>
#include <spline/Function.h>
#include "Experiment.h"
#include <Model/HumanoidFloatingModel.hpp>
#include <IKWalk/IKWalk.hpp>

class ExperimentIKWalk : public Experiment
{
    public:
        ExperimentIKWalk();
        static void initParameters(Parameters &parameters, Metabot::Robot *robot);
        void init(Simulation *simulation, Parameters &parameters);

    protected:
        Leph::HumanoidFloatingModel model;
        Leph::IKWalk::Parameters params;
};
