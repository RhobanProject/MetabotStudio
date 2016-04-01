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
    names.push_back("a_hip_pitch");
    names.push_back("a_ankle_pitch");
    names.push_back("a_hip_pitch");
    names.push_back("a_hip_yaw");
    names.push_back("a_knee");

    names.push_back("b_hip_pitch");
    names.push_back("b_ankle_pitch");
    names.push_back("b_hip_pitch");
    names.push_back("b_hip_yaw");
    names.push_back("b_hip_roll");

    names.push_back("roll");

    return names;
}
        
void ExperimentShoot::initParameters(Parameters &parameters, Metabot::Robot *robot)
{
    for (auto name : splineNames()) {
        for (int t=1; t<=5; t++) {
            std::stringstream ss;
            ss << name << "_" << t;
            double min = -150;
            double max = 150;
            if (name == "a_hip_pitch" || name == "b_hip_pitch") max = 25;
            parameters.add(ss.str(), min, max, 0);
        }
    }

    parameters.add("file", 0, 1, 0, false);

    parameters.add("freq", 0, 4, 1.95, false);
    parameters.add("rise", 0, 1, 0.025, false);
    parameters.add("step", -1, 1, 0.0, false);
    parameters.add("swing", 0, 1, 0.01, false);
    parameters.add("swingPhase", 0, 1, 0.0, false);
    parameters.add("footY", -1, 1, 0.02, false);
    parameters.add("x", -1, 1, 0.01, false);
    parameters.add("z", 0, 1, 0.03, false);
    parameters.add("pitch", -30, 30, 15, false);
    parameters.add("roll", -30, 30, 0, false);
    parameters.add("air", 0, 1, 0, false);
}

std::map<int, TransformMatrix> initStates;
bool passed = false;
        
void ExperimentShoot::init(Parameters &parameters, Metabot::Robot *robot)
{
    // Cost and collisions
    cost = 0;
    ct = 0;
    st = 0;
    collisions = 0;
    maxHeight = 0;
    trigger = false;
    shooting = false;

    // Loading splines
    for (auto name : splineNames()) {
        Function f;
        f.addPoint(0, 0);
        for (int t=1; t<=6; t++) {
            std::stringstream ss;
            ss << name << "_" << t;
            f.addPoint(t, parameters.get(ss.str()));
        }
        f.addPoint(2, 0);
        splines[name] = f;
    }

    if (parameters.get("file") > 0.5) {
        splines = Function::fromFile("shoot.json");
    }

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

    angles[LEFT_SHOULDER_ROLL] = 23;
    angles[RIGHT_SHOULDER_ROLL] = -23;

    air = parameters.get("air")>0.5;
    if (air) {
        robot->root->body->setMassProps(0, btVector3(1,1,1));
    }
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

    if (ct >= 0.01) {
        ct -= 0.01;
        if (Leph::IKWalk::walk(model, params, st, 0.01)) {
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
            
        }
        if (simulation->t > 3/params.freq && !trigger) {
            trigger = true;
            shooting = true;
            slowmo = true;
            shootT = st;
            factorSave = simulation->factor;
            simulation->factor = 0.15;
        }
        if (slowmo && simulation->t>4/params.freq) {
            simulation->factor = factorSave;
            slowmo = false;
        }

        if (shooting) {
            double splineT = (st-shootT)*12;
            angles[RIGHT_HIP_PITCH] += splines["a_hip_pitch"].get(splineT);
            angles[RIGHT_ANKLE_PITCH] -= splines["a_ankle_pitch"].get(splineT);
            angles[RIGHT_HIP_YAW] += splines["a_hip_yaw"].get(splineT);
            angles[RIGHT_KNEE] += splines["a_knee"].get(splineT);
            
            angles[LEFT_HIP_PITCH] += splines["b_hip_pitch"].get(splineT);
            angles[LEFT_ANKLE_PITCH] -= splines["b_ankle_pitch"].get(splineT);
            angles[LEFT_HIP_YAW] += splines["b_hip_yaw"].get(splineT);
//            angles[LEFT_HIP_ROLL] += splines["b_hip_roll"].get(splineT);

            params.trunkRoll = DEG2RAD(splines["roll"].get(splineT));

            if (splineT > 3) {
                shooting = false;
            }
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
        
