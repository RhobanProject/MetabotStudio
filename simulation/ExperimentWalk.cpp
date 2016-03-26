#include "ExperimentWalk.h"

double ExperimentWalkEfficience::score(Simulation *simulation)
{
    // Here, we try to maximize the distance across X axis, taking in
    // account the energy cost
    auto state = simulation->robot.getState();
    return cost*collisionsPenalty()/(simulation->duration*fabs(state.x()));
}

double ExperimentWalkSpeed::score(Simulation *simulation)
{
    // Here, we try to maximize the distance across X axis, disregarding the
    // cost
    auto state = simulation->robot.getState();
    return collisionsPenalty()/(simulation->duration*fabs(state.x()));
}
