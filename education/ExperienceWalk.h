#pragma once

#include "Experience.h"

class ExperienceWalk : public Experience
{
    public:
        void init(Experience::Parameters &parameters, Metabot::Robot *robot);
        static void initParameters(Parameters &parameters, Metabot::Robot *robot);
        virtual void control(Simulation *simulation);
        double score(Simulation *simulation);

    protected:
        double amplitude;
        double collisions, cost;

        double collisionsPenalty();
};
