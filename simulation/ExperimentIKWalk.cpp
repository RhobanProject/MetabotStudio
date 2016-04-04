#include <com/Client.h>
#include "ExperimentIKWalk.h"
#include "sigmaban.h"
#include "util.h"

ExperimentIKWalk::ExperimentIKWalk()
    : model(Leph::SigmabanModel)
{
}

void ExperimentIKWalk::initParameters(Parameters &parameters, Metabot::Robot *robot)
{
    parameters.add("file", 0, 1, 0, false);
    parameters.add("shoot", 0, 1, 1, false);

    parameters.add("freq", 0, 4, 1.95, false);
    parameters.add("rise", 0, 1, 0.025, false);
    parameters.add("step", -1, 1, 0.0, false);
    parameters.add("swing", 0, 1, 0.01, false);
    parameters.add("swingPhase", 0, 1, 0.0, false);
    parameters.add("footY", -1, 1, 0.02, false);
    parameters.add("x", -1, 1, 0.01, false);
    parameters.add("y", -1, 1, 0.0, false);
    parameters.add("z", 0, 1, 0.03, false);
    parameters.add("pitch", -30, 30, 15, false);
    parameters.add("roll", -30, 30, 0, false);
}

void ExperimentIKWalk::init(Simulation *simulation, Experiment::Parameters &parameters)
{
    params.freq = parameters.get("freq");
    params.enabledGain = 1;
    params.supportPhaseRatio = 0.0;
    params.footYOffset = parameters.get("footY");
    params.stepGain = parameters.get("step");
    params.riseGain = parameters.get("rise");
    params.turnGain = 0;
    params.lateralGain = 0;

    params.trunkXOffset = parameters.get("x");
    params.trunkYOffset = parameters.get("y");
    params.trunkZOffset = parameters.get("z");
    params.trunkRoll = DEG2RAD(parameters.get("roll"));
    params.trunkPitch = DEG2RAD(parameters.get("pitch"));

    params.swingGain = parameters.get("swing");
    params.swingRollGain = 0;
    params.swingPhase = parameters.get("swingPhase");
    params.swingPause = 0;
    params.swingVel = 4;

    params.stepUpVel = 2;
    params.stepDownVel = 2;
    params.riseUpVel = 2;
    params.riseDownVel = 2;

    params.extraLeftX = 0;
    params.extraLeftY = 0;
    params.extraLeftZ = 0;
    params.extraLeftYaw = 0;
    params.extraLeftPitch = 0;
    params.extraLeftRoll = 0;
    params.extraRightX = 0;
    params.extraRightY = 0;
    params.extraRightZ = 0;
    params.extraRightYaw = 0;
    params.extraRightPitch = 0;
    params.extraRightRoll = 0;
}
