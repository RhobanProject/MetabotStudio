#pragma once

#include <map>
#include <string>
#include <spline/Function.h>
#include "Experience.h"

#define LEFT_HIP_YAW            11
#define LEFT_HIP_ROLL           12
#define LEFT_HIP_PITCH          13
#define LEFT_KNEE               14
#define LEFT_ANKLE_PITCH        15
#define LEFT_ANKLE_ROLL         16
#define RIGHT_HIP_YAW           2
#define RIGHT_HIP_ROLL          3
#define RIGHT_HIP_PITCH         4
#define RIGHT_KNEE              5
#define RIGHT_ANKLE_PITCH       6
#define RIGHT_ANKLE_ROLL        7
#define LEFT_SHOULDER_PITCH     17
#define LEFT_SHOULDER_ROLL      18
#define LEFT_ELBOW              19
#define RIGHT_SHOULDER_PITCH    8
#define RIGHT_SHOULDER_ROLL     9
#define RIGHT_ELBOW             10
#define HEAD_YAW                20
#define HEAD_PITCH              21

class ExperienceStandUp : public Experience
{
    public:
        static double defaultDuration();
        virtual void init(Parameters &parameters, Metabot::Robot *robot);
        virtual void control(Simulation *simulation);

    protected:
        std::map<int, float> angles;
        std::map<std::string, Function> splines;

        double getAngle(int index);
};
