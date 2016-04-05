#pragma once

#include <map>
#include <string>
#include <spline/Function.h>
#include "ExperimentIKWalk.h"
#include <Model/HumanoidFloatingModel.hpp>
#include <IKWalk/IKWalk.hpp>

class ExperimentShoot : public ExperimentIKWalk
{
    public:
        ExperimentShoot();
        static std::vector<std::string> splineNames();
        static double defaultDuration();
        static void initParameters(Parameters &parameters, Metabot::Robot *robot);
        void init(Simulation *simulation, Parameters &parameters);
        virtual bool end(Simulation *simulation);
        virtual void control(Simulation *simulation);
        double score(Simulation *simulation);
        double collisionsPenalty();

    protected:
        std::vector<Vect> shoots;
        TransformMatrix shootFrame;
        int iteration;
        btRigidBody *ball;
        bool enableShoot, trigger, shooting, slowmo;
        bool air;
        double ct, st, shootT, factorSave;
        double baseFreq, shootFreq;
        std::map<int, float> angles;
        std::map<std::string, Function> splines;

        void makeBall(Simulation *simulation);
        double getAngle(int index);

        double cost, collisions, maxHeight;
};
