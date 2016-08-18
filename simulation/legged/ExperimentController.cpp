#include <verbose.h>
#include "ExperimentController.h"

ExperimentController::~ExperimentController()
{
    if (controller) {
        delete controller;
    }
}

void ExperimentController::initParameters(Parameters &parameters, Metabot::Robot *robot)
{
    // Posture parameters
    parameters.add("x", 0, 3, 0.8);
    parameters.add("y", 0, 3, 0.8);
    parameters.add("z", 0, 300, 50);

    // Controller parameters
    parameters.add("freq", 0, 4, 2);
    parameters.add("lX", -1, 1, 0);
    parameters.add("lH", 0, 100, 20);
    parameters.add("lS", 0, 10, 1);
    parameters.add("support", 0, 1, 0.5);
    parameters.add("dx", -300, 300, 60);
    parameters.add("dy", -300, 300, 0, false);
    parameters.add("turn", -3, 3, 0, false);
    parameters.add("swing", -100, 100, 0);
    parameters.add("swingPhase", 0, 1, 0);

    // Leg phases
    for (int k=1; k<=robot->tips(); k++) {
        std::stringstream p;
        p << "p" << k;
        parameters.add(p.str(), 0, 1, k%2 ? 0 : 0.5, k>1);
    }
}

void ExperimentController::init(Simulation *simulation, Experiment::Parameters &parameters)
{
    if (isVerbose()) std::cout << "Initializing the controller..." << std::endl;
    controller = new Controller(&simulation->robot);
    controller->x = parameters.get("x");
    controller->y = parameters.get("y");
    controller->z = parameters.get("z");
    controller->freq = parameters.get("freq");
    controller->lX = parameters.get("lX");
    controller->lS = parameters.get("lS");
    controller->lH = parameters.get("lH");
    controller->support = parameters.get("support");
    controller->dx = parameters.get("dx");
    controller->dy = parameters.get("dy");
    controller->turn = parameters.get("turn");
    controller->swingGain = parameters.get("swing");
    controller->swingPhase = parameters.get("swingPhase");
    controller->setupFunctions();

    for (int k=1; k<=simulation->robot.tips(); k++) {
        std::stringstream p;
        p << "p" << k;
        controller->phases[k-1] = parameters.get(p.str());
    }

    cost = 0;
    collisions = 0;        
}

void ExperimentController::control(Simulation *simulation)
{
    cost += controller->update(simulation->dt, simulation->t, simulation->robot);
    collisions += simulation->robot.world.getAutoCollisions()
        + simulation->robot.world.getGroundNonTipCollisions();

    /*
    static Vect cop(0, 0, 0);
    auto st = simulation->robot.root->getState();
    auto pos = simulation->robot.getBulletCOM();
    auto tmp = cop.multiply(0.97);
    cop = simulation->robot.getCollisionsCOP().multiply(0.03).add(tmp);
    std::cout << simulation->t << " " 
        // 2 3 4
        << pos.x() << " " << pos.y() << " " << pos.z() << " " <<
        // 5 6 7
        cop.x() << " " << cop.y() << " " << cop.z() << " " <<
        // 8 9 10
        st.x()/1000.0 << " " << st.y()/1000.0 << " " << st.z()/1000.0 << std::endl;
    //std::cout << simulation->robot.comWithinSupport() << std::endl;
    //
    */

    /*
    static double xs = 0, ys = 0;
    auto collisions = simulation->robot.world.getGroundCollisions();
    double x = 0, y = 0;
    double period = 1.0/controller->freq;
    double t = simulation->t - floor(simulation->t/period)*period;
    for (auto collision : collisions) {
        x += collision.second.x();
        y += collision.second.y();
    }
    xs = 0.9*xs + 0.1*x;
    ys = 0.9*ys + 0.1*y;
    printf("%g %g %g\n", t, xs, ys);
    */
}
        
double ExperimentController::collisionsPenalty()
{
    return (1 + pow(collisions, 4));
}
