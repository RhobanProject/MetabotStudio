#include "ExperienceWalk.h"

double ExperienceWalkEfficience::score(Simulation *simulation)
{
    auto state = simulation->robot.getState();
    // Here, we try to maximize the distance across X axis, taking in
    // account the energy cost
    return cost*pow(collisions, 2)/(simulation->duration*fabs(state.x()));
}

double ExperienceWalkSpeed::score(Simulation *simulation)
{
    auto state = simulation->robot.getState();
    // Here, we try to maximize the distance across X axis
    return pow(collisions, 2)/(simulation->duration*fabs(state.x()));
}
