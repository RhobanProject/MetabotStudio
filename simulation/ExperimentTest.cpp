#include "ExperimentTest.h"

void ExperimentZero::control(Simulation *simulation)
{
    simulation->robot.foreachComponent([simulation](Metabot::Component *component, TransformMatrix m) {
        component->setTarget(0.0, simulation->dt);
    });
}

void ExperimentSinus::control(Simulation *simulation)
{
    simulation->robot.foreachComponent([simulation](Metabot::Component *component, TransformMatrix m) {
        component->setTarget(sin(simulation->t), simulation->dt);
    });
}
