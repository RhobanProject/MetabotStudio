#include <unistd.h>
#include <Component.h>
#include "Simulation.h"
#include "util.h"
#include "verbose.h"

Simulation::Simulation(float duration, Metabot::Server *server,
        Metabot::Robot &robot, Controller &controller, double dt, int experience)
    : duration(duration), server(server), robot(robot), controller(controller), dt(dt), experience(experience)
{
    over = false;
    robot.toBullet();
    factor = 10.0;

    if (experience == 2) {
        currentCheckPoint = 0;
        maxStep = controller.dx;
        maxTurn = controller.turn;
        checkpoints.push_back(FPoint2(500, 0.0));
        checkpoints.push_back(FPoint2(500, 500));
        checkpoints.push_back(FPoint2(250, -250));
        server->updateMarker(checkpoints[0].x, checkpoints[0].y);
        controlT = 0;
    } else {
        server->disableMarker();
    }
}

void Simulation::control(double dt)
{
    if (experience == 2) {
        controlT += dt;
        if (controlT > 0.02) {
            // Updating control orders
            controlT -= 0.02;

            // Getting robot state
            auto checkpoint = checkpoints[currentCheckPoint];
            auto state = robot.getState();
            auto angles = state.toRPY();

            // Getting the target vector
            double dX = checkpoint.x-state.x();
            double dY = checkpoint.y-state.y();
            double distance = sqrt(dX*dX + dY*dY);
            if (distance < 100) {
                if (currentCheckPoint < checkpoints.size()-1) {
                    currentCheckPoint++;
                    server->updateMarker(checkpoints[currentCheckPoint].x, checkpoints[currentCheckPoint].y);
                } else {
                    over = true;
                }
            } else {
                double theta = atan2(dY, dX);
                double error = theta-angles.z();
                while (error < -M_PI) error += 2*M_PI;
                while (error > M_PI) error -= 2*M_PI;

                // The turn is the error 
                controller.turn = error*0.7;
                if (controller.turn < -maxTurn) controller.turn = -maxTurn;
                if (controller.turn > maxTurn) controller.turn = maxTurn;

                // The stepping is maxStep multiplied with error cosine
                controller.dx = maxStep*cos(error);
                if (controller.dx < 0) controller.dx = 0;
            }
        }
    }
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

    for (;!over && controllerTime < duration; controllerTime += dt) {
        control(dt);
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

        // auto state = robot.getState();
        // std::cout << state.toRPY().z() << std::endl;
    }

    return cost*pow(collisions, 2);
}
