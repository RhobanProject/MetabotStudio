#include "ExperienceWalk.h"

double ExperienceWalkEfficience::score(Simulation *simulation)
{
    // Here, we try to maximize the distance across X axis, taking in
    // account the energy cost
    auto state = simulation->robot.getState();
    return cost*collisionsPenalty()/(simulation->duration*fabs(state.x()));
}

double ExperienceWalkSpeed::score(Simulation *simulation)
{
    // Here, we try to maximize the distance across X axis, disregarding the
    // cost
    auto state = simulation->robot.getState();
    return collisionsPenalty()/(simulation->duration*fabs(state.x()));
}
