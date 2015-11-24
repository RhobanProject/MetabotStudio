#pragma once

#include <Robot.h>
#include <com/Server.h>

class Simulation
{
    public:
        class Controller
        {
            public:
                virtual void update(float duration, Metabot::Robot &robot)=0;
        };

        Simulation(float duration, Metabot::Server &server, Metabot::Robot &robot,
                Controller &controller);
        void run();
        
        float factor;

    protected:
        float duration;
        Metabot::Robot &robot;
        Metabot::Server &server;
        Controller &controller;
};
