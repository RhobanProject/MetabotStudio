#ifndef _METABOT_MOTION_H
#define _METABOT_MOTION_H

#include "Function.h"

#define GAIT_WALK       0
#define GAIT_TROT       1

class Controller
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
