#pragma once

#include <functional>
#include <Robot.h>
#include <com/Server.h>
#include <3d/Point3.h>

class Simulation
{
    public:
        Simulation(float duration, Metabot::Server *server, Metabot::Robot &robot, double dt);

        void run(std::function<void (Simulation*)> control);

        // Is the simulation over?
        bool over;
        // Simulation current time
        double t;
        // Simulation delta time
        double dt;
        // Max duration of the simulation 
        float duration;
        // Real-time factor
        float factor;

        Metabot::Robot &robot;
        Metabot::Server *server;
};
