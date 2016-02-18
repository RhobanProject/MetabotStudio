#include <unistd.h>
#include <Component.h>
#include "Simulation.h"
#include "util.h"
#include "verbose.h"

Simulation::Simulation(float duration, Metabot::Server *server,
        Metabot::Robot &robot, Controller &controller, double dt)
    : duration(duration), server(server), robot(robot), controller(controller), dt(dt)
{
    robot.toBullet();
    factor = 10.0;
}

double Simulation::run()
{
    float realTimeStart = getTime();
    float controllerTime = 0.0;
    float simTime = 0.0;
    float lastUpdate = 0.0;
    double cost = 0;
    double collisions = 1.0;

    int k = 0;
    if (isVerbose()) std::cout << "Starting simulation..." << std::endl;

    for (;controllerTime < duration; controllerTime += dt) {
        cost += controller.update(dt, controllerTime, robot);
        robot.world.stepSimulation(dt);
        collisions += robot.world.getAutoCollisions();
        //printf("%g %g %g %g\n", controllerTime-(int)controllerTime, force.x(), force.y(), force.z());
        simTime += dt/factor;
        
        float realTime = (getTime()-realTimeStart);
        if ((realTime - lastUpdate) > 0.02) {
            lastUpdate += 0.02;
            if (server) server->updateRobot(&robot);
        }
        
        if (realTime < simTime) {
            usleep(1000000*(simTime-realTime));
        }
    }

    return cost*pow(collisions, 2);
}
