#include <getopt.h>
#include <sstream>
#include <deque>
#include <map>
#include <unistd.h>
#include <Robot.h>
#include <Backend.h>
#include <com/Server.h>
#include "Controller.h"
#include "util.h"
#include "verbose.h"

#include <iostream>

static void usage()
{
    std::cout << "Metabot simulation (w/ Gazebo)" << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "  -r [file.robot]: The robot file to simulate" << std::endl;
    exit(1);
}

int main(int argc, char *argv[])
{
    bool realTime = false;
    int index;
    std::string robotFile = "";

    while ((index = getopt(argc, argv, "r:vt")) != -1) {
        switch (index) {
            case 'r':
                robotFile = std::string(optarg);
                break;
            case 'h':
                usage();
                break;
            case 'v':
                setVerbosity(1);
                break;
            case 't':
                realTime = true;
                break;
        }
    }

    if (robotFile == "") {
        usage();
    }

    // Starting the server
    Metabot::Server server;

    try { 
        // Loading the robot
        Metabot::Robot robot;
        if (isVerbose()) std::cout << "* Reading file..." << std::endl;
        robot.loadFromFile(robotFile);

        for (float l1=30; l1<100; l1+=20) {
            if (isVerbose()) std::cout << "* Exporting to bullet..." << std::endl;
            robot.parameters.set("L1", l1);
            if (isVerbose()) std::cout << "* Compiling..." << std::endl;
            robot.compile();
            if (isVerbose()) std::cout << "* Computing dynamics..." << std::endl;
            robot.computeDynamics();
            //robot.printDynamics();
            if (isVerbose()) std::cout << "* Exporting it to bullet..." << std::endl;
            robot.toBullet();
            if (isVerbose()) std::cout << "* Publishing the robot..." << std::endl;
            server.loadRobot(&robot);

            if (isVerbose()) std::cout << "Initializing the controller..." << std::endl;
            Controller controller;
            controller.dx = 60;
            controller.freq = 2.0;

            float rT = getTime();
            float t = 0.0;
            float lastUpdate = 0.0;
            int k = 0;
            if (isVerbose()) std::cout << "Starting simulation..." << std::endl;
            while (t<10.0) {
                auto angles = controller.compute(t);
                for (int k=0; k<4; k++) {
                    int leg = (k+2)%4;
                    robot.getComponentById(k*3+2)->setTarget(angles.l1[leg]);
                    robot.getComponentById(k*3+3)->setTarget(-angles.l2[leg]);
                    robot.getComponentById(k*3+4)->setTarget(angles.l3[leg]);
                }
                robot.world.stepSimulation(0.001);
                t += 0.001;
                // usleep(10000);
                float elapsed = (getTime()-rT);
                if ((elapsed - lastUpdate) > 0.02) {
                    lastUpdate += 0.02;
                    server.updateRobot(&robot);
                    k = 0;
                    float factor = t/(elapsed);
                    // std::cout << t << " " << (elapsed) << " (factor=" << factor << ")" << std::endl;
                }
                if (realTime) {
                    if (elapsed < t) {
                        usleep(1000000*(t-elapsed));
                    }
                }
            }
            auto state = robot.getState();
            auto score = sqrt(state.x()*state.x() + state.y()*state.y() + state.z()*state.z());
            std::cout << l1 << " " << score << std::endl;
        }

        /*
        for (double dx=75; dx<80; dx+=1) {
            // Initializing the controller

            float t = 0.0;
            if (isVerbose()) std::cout << "Starting simulation..." << std::endl;
            while (t < 1) {
                t += 0.02*controller.freq*metabot.factor;
                auto angles = controller.compute(t);
                for (int k=0; k<4; k++) {
                    metabot.setJoint(joints[k*3], -angles.l1[k]);
                    metabot.setJoint(joints[k*3+1], angles.l2[k]);
                    metabot.setJoint(joints[k*3+2], -angles.l3[k]);
                }
                // std::cout << std::endl;
                usleep(20000);
            }
            double d = metabot.stopMonitoring();
            metabot.unload();
            
            std::cout << dx << " " << d << std::endl;
        }
        */

    } catch (std::string err) {
        std::cerr << "Error: " << err << std::endl;
    }
}
