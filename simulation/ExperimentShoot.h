#pragma once

#include <map>
#include <string>
#include <spline/Function.h>
#include "Experiment.h"
#include <Model/HumanoidFloatingModel.hpp>
#include <IKWalk/IKWalk.hpp>

class ExperimentShoot : public Experiment
{
    public:
        ExperimentShoot();
        static std::vector<std::string> splineNames();
        static double defaultDuration();
        static void initParameters(Parameters &parameters, Metabot::Robot *robot);
        virtual void init(Parameters &parameters, Metabot::Robot *robot);
        virtual void control(Simulation *simulation);
        double score(Simulation *simulation);
        double collisionsPenalty();

    protected:
        bool air;
        double ct, st;
        Leph::HumanoidFloatingModel model;
        Leph::IKWalk::Parameters params;
        std::map<int, float> angles;
        std::map<std::string, Function> splines;

        double getAngle(int index);

        double cost, collisions, maxHeight;
};
