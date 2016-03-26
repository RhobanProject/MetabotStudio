#include "ExperimentTest.h"

void ExperimentZero::control(Simulation *simulation)
{
    simulation->robot.foreachComponent([simulation](Metabot::Component *component) {
        component->setTarget(0.0, simulation->dt);
    });
}

void ExperimentSinus::control(Simulation *simulation)
{
    simulation->robot.foreachComponent([simulation](Metabot::Component *component) {
        component->setTarget(sin(simulation->t), simulation->dt);
    });
}
