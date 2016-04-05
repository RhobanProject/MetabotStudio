#pragma once

#include <map>
#include <string>
#include <spline/Function.h>
#include "ExperimentIKWalk.h"
#include <Model/HumanoidFloatingModel.hpp>
#include <IKWalk/IKWalk.hpp>

class ExperimentStaticShoot : public ExperimentIKWalk
{
    public:
        ExperimentStaticShoot();
        static std::vector<std::string> splineNames();
        static double defaultDuration();
        static void initParameters(Parameters &parameters, Metabot::Robot *robot);
        void init(Simulation *simulation, Parameters &parameters);
        virtual bool end(Simulation *simulation);
        virtual void control(Simulation *simulation);
        double score(Simulation *simulation);
        double collisionsPenalty();

    protected:
        TransformMatrix lastState;
        double trunkSpeed;
        double left, right;
        double fallT;
        std::vector<Vect> shoots;
        TransformMatrix shootFrame;
        int iteration;
        btRigidBody *ball;
        double ct, st, shootT, factorSave;
        std::map<int, float> angles;
        std::map<std::string, Function> splines;

        void makeBall(Simulation *simulation);
        double getAngle(int index);
        bool fallen(Simulation *simulation);

        double cost, collisions, maxHeight;
};
