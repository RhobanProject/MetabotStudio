#pragma once

#include <map>
#include <string>
#include <spline/Function.h>
#include "Experiment.h"

class ExperimentStandUp : public Experiment
{
    public:
        ExperimentStandUp();
        static std::vector<std::string> splineNames();
        static double defaultDuration();
        static void initParameters(Parameters &parameters, Metabot::Robot *robot);
        void init(Parameters &parameters, Metabot::Robot *robot);
        bool end(Simulation *simulation);
        void control(Simulation *simulation);
        double score(Simulation *simulation);
        double collisionsPenalty();

    protected:
        int iteration;
        double factor;
        std::map<int, float> angles;
        std::map<std::string, Function> splines;

        double getAngle(int index);

        double cost, collisions, maxHeight;
};
