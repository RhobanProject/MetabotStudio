#ifndef _METABOT_MOTION_H
#define _METABOT_MOTION_H

#include <Robot.h>
#include "Simulation.h"
#include "Function.h"
#include "Robot.h"

#define GAIT_WALK       0
#define GAIT_TROT       1

class Controller : public Simulation::Controller
{
    public:
        // A leg of the robot
        struct Leg
        {
            Leg(Metabot::Kinematic::Tip tip);
            void gotoXYZ_rand(float x, float y, float z);
            void gotoXYZ_cmaes(float x, float y, float z);
            void gotoXYZ(float x, float y, float z);
            float error(std::vector<double> &candidate, float x, float y, float z);

            std::vector<double> alphas;

            // Leg vector and angle
            float xVec, yVec;
            float theta;

            // A kinematic tip
            Metabot::Kinematic::Tip tip;
        };

        Controller(Metabot::Robot *robot, float l1, float l2, float l3);

        // Robot parameters
        float l1, l2, l3;

        // Initializes the motion
        void setupFunctions();
        void compute(float t);

        // Compute angles & update the robot
        double update(float dt, float t, Metabot::Robot &robot);

        // Resets the motion
        void reset();

        // Parameters
        float x, y, z;
        float freq, alt;
        int gait;

        // Dynamic parameters
        float dx, dy, turn;

        // State
        float t;
        float ut;

        Function rise;
        Function step;

        // Legs phases
        float *phases;

    protected:
        std::vector<Leg> legs;
        Metabot::Robot *robot;
};

#endif
