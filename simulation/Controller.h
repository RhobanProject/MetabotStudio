#ifndef _METABOT_MOTION_H
#define _METABOT_MOTION_H

#include <Robot.h>
#include "Simulation.h"
#include "Function.h"

#define GAIT_WALK       0
#define GAIT_TROT       1

class Controller : public Simulation::Controller
{
    public:
        struct Angles
        {
            // Output angles
            float l1[4];
            float l2[4];
            float l3[4];
        };

        Controller();

        // Initializes the motion
        void setupFunctions();
        Angles compute(float t);

        // Compute angles & update the robot
        void update(float t, Metabot::Robot &robot);

        // Resets the motion
        void reset();

        // Parameters
        float freq, h, r, alt;
        int gait;

        // Dynamic parameters
        float dx, dy, turn;

        // State
        float t;

        Function rise;
        Function step;
};

#endif
