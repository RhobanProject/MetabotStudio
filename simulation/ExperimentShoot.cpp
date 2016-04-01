#include "ExperimentShoot.h"
#include "sigmaban.h"

#define RAD2DEG(a) ((a)*180.0/M_PI)
#define DEG2RAD(a) ((a)*M_PI/180.0)

ExperimentShoot::ExperimentShoot()
    : model(Leph::SigmabanModel)
{
}
        
std::vector<std::string> ExperimentShoot::splineNames()
{
    std::vector<std::string> names;
    return names;
}
        
void ExperimentShoot::initParameters(Parameters &parameters, Metabot::Robot *robot)
{
    for (auto name : splineNames()) {
        for (int t=1; t<=5; t++) {
            /*
            std::stringstream ss;
            ss << name << "_" << t;
            double min = -150;
            double max = 150;
            if (name == "elbow") max = 1;
            if (name == "knee") min = -1;
            if (name == "hip_pitch") max = 25;
            parameters.add(ss.str(), min, max, 0);
            */
        }
    }

    parameters.add("freq", 0, 4, 1.8);
    parameters.add("rise", 0, 1, 0.02);
    parameters.add("step", 0, 1, 0.0);
    parameters.add("swing", 0, 1, 0.02);
    parameters.add("swingPhase", 0, 1, 0.025);
    parameters.add("footY", 0, 1, 0.025);
    parameters.add("x", 0, 1, 0.01);
    parameters.add("z", 0, 1, 0.02);
    parameters.add("pitch", 0, 30, 10);
    parameters.add("roll", 0, 30, 0);
    parameters.add("air", 0, 1, 0);
}

std::map<int, TransformMatrix> initStates;
bool passed = false;
        
void ExperimentShoot::init(Parameters &parameters, Metabot::Robot *robot)
{
    robot->root->body->setMassProps(0, btVector3(1.5,1.5,1.5));

    // Cost and collisions
    cost = 0;
    ct = 0;
    st = 0;
    collisions = 0;
    maxHeight = 0;

    /*
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
    //splines = Function::fromFile("standup.json");
    */

    params.freq = parameters.get("freq");
    params.enabledGain = 1;
    params.supportPhaseRatio = 0.0;
    params.footYOffset = parameters.get("footY");
    params.stepGain = parameters.get("step");
    params.riseGain = parameters.get("rise");
    params.turnGain = 0;
    params.lateralGain = 0;

    params.trunkXOffset = parameters.get("x");
    params.trunkYOffset = 0.0;
    params.trunkZOffset = parameters.get("z");
    params.trunkRoll = DEG2RAD(parameters.get("roll"));
    params.trunkPitch = DEG2RAD(parameters.get("pitch"));

    params.swingGain = parameters.get("swing");
    params.swingRollGain = 0;
    params.swingPhase = parameters.get("swingPhase");
    params.swingPause = 0;
    params.swingVel = 4;

    params.stepUpVel = 2;
    params.stepDownVel = 2;
    params.riseUpVel = 2;
    params.riseDownVel = 2;

    params.extraLeftX = 0;
    params.extraLeftY = 0;
    params.extraLeftZ = 0;
    params.extraLeftYaw = 0;
    params.extraLeftPitch = 0;
    params.extraLeftRoll = 0;
    params.extraRightX = 0;
    params.extraRightY = 0;
    params.extraRightZ = 0;
    params.extraRightYaw = 0;
    params.extraRightPitch = 0;
    params.extraRightRoll = 0;

    angles[LEFT_SHOULDER_ROLL] = 15;
    angles[RIGHT_SHOULDER_ROLL] = -15;

    air = parameters.get("air")>0.5;
}

void ExperimentShoot::control(Simulation *simulation)
{
    if (simulation->t > 1 && !passed) {
        passed = true;
        auto comp = simulation->robot.root;
        auto body = comp->body;
        if (!air) {
          body->setMassProps(comp->mass, comp->inertia);
        }
    }
        ct += simulation->dt;

    if (ct > 0.02) {
        ct -= 0.02;
        if (Leph::IKWalk::walk(model, params, st, 0.02)) {
            angles[LEFT_HIP_YAW] = RAD2DEG(model.getDOF("left_hip_yaw"));
            angles[LEFT_HIP_ROLL] = RAD2DEG(model.getDOF("left_hip_roll"));
            angles[LEFT_HIP_PITCH] = RAD2DEG(model.getDOF("left_hip_pitch"));
            angles[LEFT_KNEE] = RAD2DEG(model.getDOF("left_knee"));
            angles[LEFT_ANKLE_ROLL] = -RAD2DEG(model.getDOF("left_ankle_roll"));
            angles[LEFT_ANKLE_PITCH] = -RAD2DEG(model.getDOF("left_ankle_pitch"));
            
            angles[RIGHT_HIP_YAW] = RAD2DEG(model.getDOF("right_hip_yaw"));
            angles[RIGHT_HIP_ROLL] = RAD2DEG(model.getDOF("right_hip_roll"));
            angles[RIGHT_HIP_PITCH] = RAD2DEG(model.getDOF("right_hip_pitch"));
            angles[RIGHT_KNEE] = RAD2DEG(model.getDOF("right_knee"));
            angles[RIGHT_ANKLE_ROLL] = RAD2DEG(model.getDOF("right_ankle_roll"));
            angles[RIGHT_ANKLE_PITCH] = -RAD2DEG(model.getDOF("right_ankle_pitch"));

            std::cout << "~~~" << std::endl;
            std::cout << angles[LEFT_HIP_ROLL] << "/";
            std::cout << angles[RIGHT_HIP_ROLL] << std::endl;
            std::cout << angles[LEFT_HIP_ROLL] << "/";
            std::cout << angles[RIGHT_HIP_ROLL] << std::endl;
        }
    }
    
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
        
double ExperimentShoot::getAngle(int index)
{
    if (angles.count(index)) {
        return angles[index]*M_PI/180.0;
    } else {
        return 0.0;
    }
}
        
double ExperimentShoot::score(Simulation *simulation)
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

double ExperimentShoot::defaultDuration()
{
    return 20;
}

double ExperimentShoot::collisionsPenalty()
{
    return (1 + pow(collisions, 2));
}
        
