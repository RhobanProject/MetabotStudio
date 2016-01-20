#pragma once

#include <Robot.h>
#include <com/Server.h>

class Simulation
{
    public:
        class Controller
        {
            public:
                virtual double update(float dt, float duration, Metabot::Robot &robot)=0;
        };

        Simulation(float duration, Metabot::Server *server, Metabot::Robot &robot,
                Controller &controller, double dt);
        double run();
        
        float factor;

    protected:
        double dt;
        float duration;
        Metabot::Robot &robot;
        Metabot::Server *server;
        Controller &controller;
};
