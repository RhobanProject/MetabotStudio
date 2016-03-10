#include "ExperienceWalk.h"
        
void ExperienceWalk::initParameters(Parameters &parameters, Metabot::Robot *robot)
{
    parameters.add("amplitude", 0, 2, 1, true);
}
        
void ExperienceWalk::init(Experience::Parameters &parameters, Metabot::Robot *robot)
{
    collisions = 0;
    cost = 0;
    amplitude = parameters.get("amplitude");
}

void ExperienceWalk::control(Simulation *simulation)
{
    std::vector<double> angles(12, 0.0);
    double t = simulation->t;

    for (int k=0; k<12; k++) {
        angles[k] = sin(t)*amplitude;
    }

    cost += setMotorTargets(simulation, angles);
    
    collisions += simulation->robot.world.getAutoCollisions();
}

double ExperienceWalk::collisionsPenalty()
{
    return (1 + pow(collisions, 2));
}
    
double ExperienceWalk::score(Simulation *simulation)
{
    auto state = simulation->robot.getState();
    return cost*collisionsPenalty()/(simulation->duration*fabs(state.x()));
}
