#include "ExperimentStandUp.h"
#include "sigmaban.h"
        
ExperimentStandUp::ExperimentStandUp()
{
    iteration = 1;
}
        
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
    parameters.add("file", 0, 1, 0, false);
    parameters.add("factor", 0, 10, 1.0, false);
    parameters.add("back", 0, 1, 0, false);
    
    auto seed = Function::fromFile("seed.json");
    for (auto name : splineNames()) {
        for (int t=1; t<=5; t++) {
            std::stringstream ss;
            ss << name << "_" << t;
            double min = -150;
            double max = 150;
            if (name == "elbow") max = 1;
            if (name == "knee") min = -1;
            if (name == "hip_pitch") max = 25;
            parameters.add(ss.str(), min, max, seed[name].get(t));
            // parameters.randomize(ss.str());
        }
    }
}
        
void ExperimentStandUp::init(Simulation *simulation, Experiment::Parameters &parameters)
{
    auto robot = &simulation->robot;
    // Cost and collisions
    cost = 0;
    collisions = 0;
    maxHeight = 0;
    factor = parameters.get("factor");

    // Sets the friction on the arms
    robot->getComponentById(LEFT_ELBOW)->body->setFriction(0.6);
    robot->getComponentById(RIGHT_ELBOW)->body->setFriction(0.6);

    bool back = parameters.get("back")>0.5;

    // Put the robot on the front at the begining
    robot->foreachComponent([back](Metabot::Component *component) {
        if (component->body) {
            auto trans = TransformMatrix::fromBullet(component->body->getWorldTransform());
            trans = TransformMatrix::rotationY(back ? -M_PI/2 : M_PI/2).multiply(trans);
            trans.values[2][3] += 100;
            component->body->setWorldTransform(trans.toBullet());
        }
    });
    
    //robot->root->body->setMassProps(0, btVector3(1,1,1));
    
    // Loading splines
    for (auto name : splineNames()) {
        Function f;
        f.addPoint(0, 0);
        for (int t=1; t<=6; t++) {
            std::stringstream ss;
            ss << name << "_" << t;
            f.addPoint(t, parameters.get(ss.str()));
        }
        f.addPoint(6, 0);
        splines[name] = f;
    }
    if (parameters.get("file")>0.5) {
        splines = Function::fromFile("standup.json");
    }
}

bool ExperimentStandUp::end(Simulation *simulation)
{
    auto state = simulation->robot.getState();
    auto rpy = state.toRPY();

    if (fabs(rpy.x()) < 0.15 && fabs(rpy.y()) < 0.15) {
        iteration++;

        return iteration >= 4;
    } else {
        return true;
    }
}

void ExperimentStandUp::control(Simulation *simulation)
{
    double t = simulation->t*factor;
    if (iteration == 2) t /= 1.5;
    if (iteration == 3) t *= 1.5;

    angles[LEFT_SHOULDER_PITCH] = -splines["shoulder_pitch"].get(t);
    angles[RIGHT_SHOULDER_PITCH] = splines["shoulder_pitch"].get(t);

    angles[LEFT_ELBOW] = -splines["elbow"].get(t);
    angles[RIGHT_ELBOW] = -splines["elbow"].get(t);
    
    angles[LEFT_ANKLE_PITCH] = -splines["ankle_pitch"].get(t);
    angles[RIGHT_ANKLE_PITCH] = -splines["ankle_pitch"].get(t);
    
    angles[LEFT_HIP_PITCH] = splines["hip_pitch"].get(t);
    angles[RIGHT_HIP_PITCH] = splines["hip_pitch"].get(t);
    
    angles[LEFT_KNEE] = splines["knee"].get(t);
    angles[RIGHT_KNEE] = splines["knee"].get(t);
   
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

    if (iteration >= 4) {
        score = cost*collisionsPenalty();
    } else {
        score += 1e7*(4-iteration);
        score += 10000/maxHeight;
        score += collisionsPenalty();
    }

    return score;
}

double ExperimentStandUp::defaultDuration()
{
    return 12;
}

double ExperimentStandUp::collisionsPenalty()
{
    return (1 + pow(collisions, 2));
}
        
