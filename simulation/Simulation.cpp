#include <unistd.h>
#include <Component.h>
#include "Simulation.h"
#include "util.h"
#include "verbose.h"

Simulation::Simulation(float duration, Metabot::Server &server,
        Metabot::Robot &robot, Controller &controller)
    : duration(duration), server(server), robot(robot), controller(controller)
{
    robot.toBullet();
    factor = 10.0;
}

void Simulation::run()
{
    float realTimeStart = getTime();
    float controllerTime = 0.0;
    float simTime = 0.0;
    float lastUpdate = 0.0;

    int k = 0;
    if (isVerbose()) std::cout << "Starting simulation..." << std::endl;

    for (;controllerTime < duration; controllerTime += 0.001) {
        controller.update(controllerTime, robot);
        robot.world.stepSimulation(0.001);
        simTime += 0.001/factor;
        
        float realTime = (getTime()-realTimeStart);
        if ((realTime - lastUpdate) > 0.02) {
            lastUpdate += 0.02;
            server.updateRobot(&robot);
        }
        
        if (realTime < simTime) {
            usleep(1000000*(simTime-realTime));
        }
    }
}
