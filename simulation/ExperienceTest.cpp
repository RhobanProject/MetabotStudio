#include "ExperienceTest.h"

void ExperienceZero::control(Simulation *simulation)
{
    simulation->robot.foreachComponent([simulation](Metabot::Component *component) {
        component->setTarget(0.0, simulation->dt);
    });
}

void ExperienceSinus::control(Simulation *simulation)
{
    simulation->robot.foreachComponent([simulation](Metabot::Component *component) {
        component->setTarget(sin(simulation->t), simulation->dt);
    });
}
