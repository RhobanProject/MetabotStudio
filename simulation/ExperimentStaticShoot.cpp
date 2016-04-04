#include <com/Client.h>
#include "ExperimentStaticShoot.h"
#include "sigmaban.h"
#include "util.h"

ExperimentStaticShoot::ExperimentStaticShoot()
{
    iteration = 1;
}

std::vector<std::string> ExperimentStaticShoot::splineNames()
{
    std::vector<std::string> names;
    names.push_back("sX");
    names.push_back("sY");
    names.push_back("sZ");
    names.push_back("sR");
    names.push_back("sP");

    return names;
}

void ExperimentStaticShoot::initParameters(Parameters &parameters, Metabot::Robot *robot)
{
    ExperimentIKWalk::initParameters(parameters, robot);

    // Going on one leg
    parameters.add("sX", -0.1, 0.1, 0);
    parameters.add("sY", -0.1, 0.1, 0);
    parameters.add("sZ", -0.1, 0.1, 0);
    parameters.add("sR", -40, 40, 0);
    parameters.add("sP", -40, 40, 0);
    parameters.add("footSpace", 0, 0.1, 0.03);
    
    // Rising leg
    parameters.add("rise", 0, 1, 0.03);
}

void ExperimentStaticShoot::makeBall(Simulation *simulation)
{
    if (!ball && iteration <= 2) {
        double ballDistance;
        double radius = 75;
        double mass = 0.15;

        if (iteration == 1) ballDistance = 190;
        if (iteration == 2) ballDistance = 150;

        shootFrame = simulation->robot.getComponentById(RIGHT_ANKLE_ROLL)->getState();

        // Creating ball
        auto &world = simulation->robot.world;
        auto shape = world.createSphere(radius/1000.0);
        btVector3 inertia;
        shape->calculateLocalInertia(mass, inertia);
        TransformMatrix offset = TransformMatrix::identity();
        offset.setX(-ballDistance);
        offset.setZ(100);
        shootFrame = shootFrame.multiply(offset);
        ball = world.createRigidBody(mass, shootFrame.toBullet(), shape, inertia);
        ball->setDamping(0, 1.5);
        if (simulation->server) simulation->server->addShape(0, COM_SHAPE_SPHERE, 
                TransformMatrix::identity(), {radius});
    }
}

void ExperimentStaticShoot::init(Simulation *simulation, Experiment::Parameters &parameters)
{
    left = 0;
    right = 0;
    fallT = 0;

    // Ball
    makeBall(simulation);

    // The ball is not still there
    ball = NULL;

    // Cost and collisions
    cost = 0;
    ct = 0;
    st = 0;
    collisions = 0;
    maxHeight = 0;

    // Loading splines
    for (auto name : splineNames()) {
        Function f;
        f.addPoint(0, 0);
        f.addPoint(3, parameters.get(name));
        splines[name] = f;
    }
    
    Function rise;
    rise.addPoint(0, 0);
    rise.addPoint(4, 0);
    rise.addPoint(5, parameters.get("rise"));
    splines["rise"] = rise;

    if (parameters.get("file") > 0.5) {
        splines = Function::fromFile("shoot.json");
    }


    this->ExperimentIKWalk::init(simulation, parameters);
    params.riseGain = 0;
    params.swingGain = 0;
    params.footYOffset = parameters.get("footSpace");

    angles[LEFT_SHOULDER_ROLL] = 23;
    angles[RIGHT_SHOULDER_ROLL] = -23;
}

bool ExperimentStaticShoot::end(Simulation *simulation)
{
    auto state = simulation->robot.getState();
    auto rpy = state.toRPY();

    if (simulation->server) simulation->server->init();

    if (ball) {
        auto st = simulation->robot.world.getState(ball);
        auto tmp = st.multiply(shootFrame.invert());
        tmp = shootFrame.invert().multiply(st);
        auto shoot = Vect(-tmp.x(), tmp.y(), tmp.z());
        shoots.push_back(shoot);
        ball = NULL;
    }

    return true;

    /*
    if (fabs(rpy.x()) < 0.35 && fabs(rpy.y()) < 0.35) {
        iteration++;

        return iteration >= 4;
    } else {
        return true;
    }
    */
}

void ExperimentStaticShoot::control(Simulation *simulation)
{
    if (fallen(simulation) && fallT<0.1) {
        fallT = simulation->t;
    }

    // Ball
    if (ball) {
        auto ballState = simulation->robot.world.getState(ball);
        if (simulation->server) simulation->server->updateShape(0, ballState);
    }

    ct += simulation->dt;

    if (ct >= 0.01) {
        params.extraLeftX = splines["sX"].get(simulation->t);
        params.extraRightY = splines["sX"].get(simulation->t);

        params.extraLeftY = splines["sY"].get(simulation->t);
        params.extraRightY = splines["sY"].get(simulation->t);

        params.extraLeftZ = splines["sZ"].get(simulation->t);
        params.extraRightZ = splines["sZ"].get(simulation->t);

        params.trunkPitch = DEG2RAD(splines["sP"].get(simulation->t));
        params.trunkRoll = DEG2RAD(splines["sR"].get(simulation->t));
        params.extraRightZ = splines["rise"].get(simulation->t);

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
    }

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

double ExperimentStaticShoot::getAngle(int index)
{
    if (angles.count(index)) {
        return angles[index]*M_PI/180.0;
    } else {
        return 0.0;
    }
}

bool ExperimentStaticShoot::fallen(Simulation *simulation)
{
    auto robot = &simulation->robot;
    auto &world = robot->world;
    double nleft = world.getGroundCollisions(robot->getComponentById(LEFT_ANKLE_ROLL)->body);
    double nright = world.getGroundCollisions(robot->getComponentById(RIGHT_ANKLE_ROLL)->body);
    auto other = world.getGroundNonTipCollisions()-nleft-nright;
    left = 0.9*left+0.1*nleft;
    right = 0.9*right+0.1*nright;

    return other>0.001;
}

double ExperimentStaticShoot::score(Simulation *simulation)
{
    double score = 0;
    
    // Fallen?
    if (fallT > 0.1) {
        return 1e6 + 1000/fallT;
    }

    // Ground force repartition
    if (right > 1e-3) {
        return 1e3+right;
    } else {
        auto state = simulation->robot.getComponentById(RIGHT_ANKLE_ROLL)->getState();
        return 1/state.z();
    }

    if (iteration >= 4) {
        for (auto shoot : shoots) {
            if (fabs(shoot.x()) < 0.1) score += 1e6;
            else score += fabs(10000/shoot.x());
        }
        score += collisionsPenalty()/25;
    } else {
        return 1e7;
    }

    return score;
}

double ExperimentStaticShoot::defaultDuration()
{
    return 6;
}

double ExperimentStaticShoot::collisionsPenalty()
{
    return (1 + pow(collisions, 2));
}

