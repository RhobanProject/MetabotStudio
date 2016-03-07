#include "ExperienceWalk.h"

void ExperienceWalk::control(Simulation *simulation)
{
    std::vector<double> angles(12, 0.0);
    double t = simulation->t;

    for (int k=0; k<12; k++) {
        angles[k] = sin(t);
    }

    setMotorTargets(simulation, angles);
}
