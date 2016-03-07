#include <unistd.h>
#include <Component.h>
#include "Simulation.h"
#include "util.h"
#include "verbose.h"

Simulation::Simulation(float duration, Metabot::Server *server,
        Metabot::Robot &robot, double dt)
    : duration(duration), server(server), robot(robot), dt(dt)
{
    over = false;
    robot.toBullet();
    factor = 100.0;
}

void Simulation::run(std::function<void (Simulation*)> control)
{
    float realTimeStart = getTime();
    float simTime = 0.0;
    float lastUpdate = 0.0;

    if (isVerbose()) std::cout << "Starting simulation..." << std::endl;

    for (t=0; !over && t < duration; t += dt) {
        // Calling control function
        control(this);

        robot.world.stepSimulation(dt);
        simTime += dt/factor;
        
        float realTime = (getTime()-realTimeStart);
        if ((realTime - lastUpdate) > 0.02) {
            lastUpdate += 0.02;
            if (server) server->updateRobot(&robot);
        }
        
        if (realTime < simTime) {
            usleep(1000000*(simTime-realTime));
        }

        // auto state = robot.getState();
        // std::cout << state.toRPY().z() << std::endl;
    }
}
