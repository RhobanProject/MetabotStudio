#include <com/Client.h>
#include "ExperimentShoot.h"
#include "sigmaban.h"
#include "util.h"

#define POINTS  3

ExperimentShoot::ExperimentShoot()
{
    iteration = 1;
}

std::vector<std::string> ExperimentShoot::splineNames()
{
    std::vector<std::string> names;
    names.push_back("a_hip_yaw");
    names.push_back("a_hip_pitch");
    names.push_back("a_hip_roll");
    names.push_back("a_knee");
    names.push_back("a_ankle_pitch");
    names.push_back("a_ankle_roll");

    names.push_back("b_hip_yaw");
    names.push_back("b_hip_pitch");
    names.push_back("b_hip_roll");
    names.push_back("b_knee");
    names.push_back("b_ankle_pitch");
    names.push_back("b_ankle_roll");

    names.push_back("roll");

    return names;
}

void ExperimentShoot::initParameters(Parameters &parameters, Metabot::Robot *robot)
{
    ExperimentIKWalk::initParameters(parameters, robot);

    for (auto name : splineNames()) {
        for (int t=1; t<=POINTS; t++) {
            std::stringstream ss;
            ss << name << "_" << t;
            double min = -150;
            double max = 150;
            parameters.add(ss.str(), min, max, 0);
        }
    }

    parameters.add("air", 0, 1, 0, false);
    parameters.add("shootFreq", 0, 3, 1.5);
    parameters.add("shootStep", 0, 3, 0.06);
}

void ExperimentShoot::makeBall(Simulation *simulation)
{
    if (!ball && iteration <= 2) {
        double ballDistance;
        double radius = 75;
        double mass = 0.200;

        if (iteration == 1) ballDistance = 190;
        if (iteration == 2) ballDistance = 150;

        shootFrame = simulation->robot.getComponentById(LEFT_ANKLE_ROLL)->getState();

        // Creating ball
        auto &world = simulation->robot.world;
        auto shape = world.createSphere(radius/1000.0);
        btVector3 inertia;
        shape->calculateLocalInertia(mass, inertia);
        TransformMatrix offset = TransformMatrix::identity();
        offset.setX(ballDistance);
        offset.setZ(80);
        shootFrame = shootFrame.multiply(offset);
        ball = world.createRigidBody(mass, shootFrame.toBullet(), shape, inertia);
        if (simulation->server) simulation->server->addShape(0, COM_SHAPE_SPHERE, 
                TransformMatrix::identity(), {radius});
    }
}

void ExperimentShoot::init(Simulation *simulation, Experiment::Parameters &parameters)
{
    // The ball is not still there
    ball = NULL;

    // Cost and collisions
    cost = 0;
    ct = 0;
    st = 0;
    collisions = 0;
    maxHeight = 0;
    trigger = false;
    shooting = false;
    slowmo = false;
    enableShoot = parameters.get("shoot")>0.5;

    // Loading splines
    for (auto name : splineNames()) {
        Function f;
        f.addPoint(0, 0);
        for (int t=1; t<=POINTS; t++) {
            std::stringstream ss;
            ss << name << "_" << t;
            f.addPoint(t/(POINTS+1.0), parameters.get(ss.str()));
        }
        f.addPoint(1, 0);
        splines[name] = f;
    }

    if (parameters.get("file") > 0.5) {
        splines = Function::fromFile("shoot.json");
    }

    this->ExperimentIKWalk::init(simulation, parameters);
    baseFreq = params.freq;
    shootFreq = parameters.get("shootFreq");
    shootStep = parameters.get("shootStep");
    
    angles[LEFT_SHOULDER_ROLL] = 23;
    angles[RIGHT_SHOULDER_ROLL] = -23;

    air = parameters.get("air")>0.5;
    if (air) {
        simulation->robot.root->body->setMassProps(0, btVector3(1,1,1));
    }
}

bool ExperimentShoot::end(Simulation *simulation)
{
    auto state = simulation->robot.getState();
    auto rpy = state.toRPY();

    if (simulation->server) simulation->server->init();

    if (ball) {
        auto st = simulation->robot.world.getState(ball);
        auto tmp = st.multiply(shootFrame.invert());
        tmp = shootFrame.invert().multiply(st);
        auto shoot = Vect(tmp.x(), tmp.y(), tmp.z());
        shoots.push_back(shoot);
        ball = NULL;
    }

    if (fabs(rpy.x()) < 0.35 && fabs(rpy.y()) < 0.35) {
        iteration++;

        return iteration >= 4;
    } else {
        return true;
    }
}

void ExperimentShoot::control(Simulation *simulation)
{
    // Ball
    if (ball) {
        auto ballState = simulation->robot.world.getState(ball);
        if (simulation->server) simulation->server->updateShape(0, ballState);
    
        auto &world = simulation->robot.world;
        auto ballGround = world.getGroundCollisions(ball);
        ball->setDamping(0, fabs(ballGround)>1e-7 ? 1 : 0);
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

        if (enableShoot) {
            double startAt = 4;
            if (simulation->t > startAt/params.freq && !trigger) {
                params.stepGain = shootStep;
            }
            if (simulation->t > (startAt+0.5)/params.freq && !trigger) {
                trigger = true;
                shooting = true;
                lastSplineT = 0;
                params.freq = shootFreq;
                shootT = st;
            }
            if (!slowmo && simulation->t > (startAt-0.3)/params.freq) {
                makeBall(simulation);
                if (simulation->factor < 1.5) {
                    slowmo = true;
                    factorSave = simulation->factor;
                    simulation->factor = 0.3;
                }
            }
            if (slowmo && simulation->t>(startAt+1+0.3)/params.freq) {
                simulation->factor = factorSave;
                slowmo = false;
            }

            if (shooting) {
                double splineT = (st-shootT)*2;
                angles[RIGHT_HIP_YAW] += splines["a_hip_yaw"].get(splineT);
                angles[RIGHT_HIP_PITCH] += splines["a_hip_pitch"].get(splineT);
                angles[RIGHT_HIP_ROLL] += splines["a_hip_roll"].get(splineT);
                angles[RIGHT_KNEE] += splines["a_knee"].get(splineT);
                angles[RIGHT_ANKLE_ROLL] -= splines["a_ankle_roll"].get(splineT);
                angles[RIGHT_ANKLE_PITCH] -= splines["a_ankle_pitch"].get(splineT);

                angles[LEFT_HIP_YAW] += splines["b_hip_yaw"].get(splineT);
                angles[LEFT_HIP_PITCH] += splines["b_hip_pitch"].get(splineT);
                angles[LEFT_HIP_ROLL] += splines["b_hip_roll"].get(splineT);
                angles[LEFT_KNEE] += splines["b_knee"].get(splineT);
                angles[LEFT_ANKLE_PITCH] -= splines["b_ankle_pitch"].get(splineT);
                angles[LEFT_ANKLE_ROLL] += splines["b_ankle_roll"].get(splineT);

                params.trunkRoll = DEG2RAD(splines["roll"].get(splineT));

                if (splineT < lastSplineT) {
                    shooting = false;
                    params.stepGain = 0.0;
                    params.freq = baseFreq;
                }
                lastSplineT = splineT;
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

double ExperimentShoot::defaultDuration()
{
    return 6;
}

double ExperimentShoot::collisionsPenalty()
{
    return (1 + pow(collisions, 2));
}

