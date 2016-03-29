#include "ExperimentStandUp.h"
        
double ExperimentStandUp::defaultDuration()
{
    return 15;
}
        
void ExperimentStandUp::init(Parameters &parameters, Metabot::Robot *robot)
{
    // Sets the friction on the arms
    robot->getComponentById(LEFT_ELBOW)->body->setFriction(0.6);
    robot->getComponentById(RIGHT_ELBOW)->body->setFriction(0.6);

    // Put the robot on the front at the begining
    robot->foreachComponent([](Metabot::Component *component) {
        if (component->body) {
            auto trans = TransformMatrix::fromBullet(component->body->getWorldTransform());
            trans = TransformMatrix::rotationY(M_PI/2).multiply(trans);
            trans.values[2][3] += 100;
            component->body->setWorldTransform(trans.toBullet());
        }
    });
    
    // Loading splines
    splines = Function::fromFile("standup.json");
}

void ExperimentStandUp::control(Simulation *simulation)
{
    angles[LEFT_SHOULDER_PITCH] = -splines["shoulder_pitch"].get(simulation->t);
    angles[RIGHT_SHOULDER_PITCH] = splines["shoulder_pitch"].get(simulation->t);

    angles[LEFT_ELBOW] = -splines["elbow"].get(simulation->t);
    angles[RIGHT_ELBOW] = -splines["elbow"].get(simulation->t);
    
    angles[LEFT_ANKLE_PITCH] = -splines["ankle_pitch"].get(simulation->t);
    angles[RIGHT_ANKLE_PITCH] = -splines["ankle_pitch"].get(simulation->t);
    
    angles[LEFT_HIP_PITCH] = splines["hip_pitch"].get(simulation->t);
    angles[RIGHT_HIP_PITCH] = splines["hip_pitch"].get(simulation->t);
    
    angles[LEFT_KNEE] = splines["knee"].get(simulation->t);
    angles[RIGHT_KNEE] = splines["knee"].get(simulation->t);
    
    angles[HEAD_PITCH] = 60;
 
    simulation->robot.foreachComponent([this, simulation](Metabot::Component *component) {
        component->setTarget(this->getAngle(component->id), simulation->dt);
    });
}
        
double ExperimentStandUp::getAngle(int index)
{
    if (angles.count(index)) {
        return angles[index]*M_PI/180.0;
    } else {
        return 0.0;
    }
}