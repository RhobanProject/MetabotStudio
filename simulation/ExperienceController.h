#pragma once

#include <Robot.h>
#include "Controller.h"
#include "Experience.h"

class ExperienceController : public Experience
{
    public:
        virtual ~ExperienceController();
        void init(Experience::Parameters &parameters, Metabot::Robot *robot);
        void control(Simulation *simulation);
        
        static void initParameters(Parameters &parameters, Metabot::Robot *robot);

    protected:
        Controller *controller;

        double cost;
        double collisions;
};
