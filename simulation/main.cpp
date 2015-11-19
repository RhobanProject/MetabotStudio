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
    int index;
    std::string robotFile = "";

    while ((index = getopt(argc, argv, "r:v")) != -1) {
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
            
        if (isVerbose()) std::cout << "* Compiling..." << std::endl;
        robot.compile();
        if (isVerbose()) std::cout << "* Computing dynamics..." << std::endl;
        robot.computeDynamics();
        if (isVerbose()) std::cout << "* Exporting to bullet..." << std::endl;
        robot.toBullet();

        if (isVerbose()) std::cout << "* Publishing the robot..." << std::endl;
        server.loadRobot(&robot);

        if (isVerbose()) std::cout << "Initializing the controller..." << std::endl;
        Controller controller;
        controller.dx = 60;
        controller.freq = 2.0;

        float rT = getTime();
        float t = 0.0;
        int k = 0;
        if (isVerbose()) std::cout << "Starting simulation..." << std::endl;
        while (true) {
            auto angles = controller.compute(t);
            for (int leg=0; leg<4; leg++) {
                robot.getComponentById(leg*3+2)->setTarget(angles.l1[leg]);
                robot.getComponentById(leg*3+3)->setTarget(-angles.l2[leg]);
                robot.getComponentById(leg*3+4)->setTarget(angles.l3[leg]);
            }
            robot.world.stepSimulation(0.001);
            usleep(1000);
            t += 0.001;
            if ((k++) > 20) {
                server.updateRobot(&robot);
                k = 0;
                float factor = t/(getTime()-rT);
                std::cout << t << " " << (getTime()-rT) << " (factor=" << factor << ")" << std::endl;
            }
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
