#include "ExperimentStandUp.h"
        
std::vector<std::string> ExperimentStandUp::splineNames()
{
    std::vector<std::string> names;
    names.push_back("shoulder_pitch");
    names.push_back("elbow");
    names.push_back("hip_pitch");
    names.push_back("knee");
    names.push_back("ankle_pitch");

    return names;
}
        
void ExperimentStandUp::initParameters(Parameters &parameters, Metabot::Robot *robot)
{
    for (auto name : splineNames()) {
        for (int t=1; t<=5; t++) {
            std::stringstream ss;
            ss << name << "_" << t;
            double min = -150;
            double max = 150;
            if (name == "elbow") max = 1;
            if (name == "knee") min = -1;
            if (name == "hip_pitch") max = 25;
            parameters.add(ss.str(), min, max, 0);
        }
    }
}
        
void ExperimentStandUp::init(Parameters &parameters, Metabot::Robot *robot)
{
    // Cost and collisions
    cost = 0;
    collisions = 0;
    maxHeight = 0;

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
    for (auto name : splineNames()) {
        Function f;
        f.addPoint(0, 0);
        for (int t=1; t<=6; t++) {
            std::stringstream ss;
            ss << name << "_" << t;
            f.addPoint(t*1.8, parameters.get(ss.str()));
        }
        f.addPoint(6*1.8, 0);
        splines[name] = f;
    }
    // splines = Function::fromFile("standup.json");
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
    
    //angles[HEAD_PITCH] = 60;
 
    simulation->robot.foreachComponent([this, simulation](Metabot::Component *component) {
        this->cost += component->setTarget(this->getAngle(component->id), simulation->dt);
    });
   
    if (simulation->t > 1) {
        collisions += simulation->robot.world.getAutoCollisions();
        auto state = simulation->robot.getComponentById(HEAD_PITCH)->getState();
        if (state.z() > maxHeight) {
            maxHeight = state.z();
        }
    }
}
        
double ExperimentStandUp::getAngle(int index)
{
    if (angles.count(index)) {
        return angles[index]*M_PI/180.0;
    } else {
        return 0.0;
    }
}
        
double ExperimentStandUp::score(Simulation *simulation)
{
    double score = 0;
    auto state = simulation->robot.getState();
    auto rpy = state.toRPY();

    if (fabs(rpy.y()) > 0.1) {
        // The standup failed
        score = 1e6 + 10000/maxHeight + collisionsPenalty();
    } else {
        score = cost*collisionsPenalty();
    }

    return score;
}

double ExperimentStandUp::defaultDuration()
{
    return 20;
}

double ExperimentStandUp::collisionsPenalty()
{
    return (1 + pow(collisions, 2));
}
        
