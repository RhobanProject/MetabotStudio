#pragma once

#include <Robot.h>
#include <com/Server.h>
#include <3d/Point3.h>
#include "Controller.h"

class Simulation
{
    public:
        Simulation(float duration, Metabot::Server *server, Metabot::Robot &robot,
                Controller &controller, double dt, int experience);

        double run();
        void control(double dt);
        double score(double duration, double cost, double collisions);
        
        float factor;

    protected:
        double controlT;
        int currentCheckPoint;
        std::vector<FPoint2> checkpoints;
        double maxStep, maxTurn;

        int experience;
        bool over;
        double dt;
        float duration;
        Metabot::Robot &robot;
        Metabot::Server *server;
        Controller &controller;
};
