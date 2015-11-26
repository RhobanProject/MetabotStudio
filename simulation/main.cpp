#include <getopt.h>
#include <sstream>
#include <deque>
#include <map>
#include <unistd.h>
#include <Robot.h>
#include <Backend.h>
#include <com/Server.h>
#include <RhIO.hpp>
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
    float factor = 10.0;
    int index;
    std::string robotFile = "";

    while ((index = getopt(argc, argv, "r:vtf:")) != -1) {
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
                factor = 1.0;
                break;
            case 'f':
                factor = atof(optarg);
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

        for (float l1=50; l1<550; l1+=20) {
            robot.parameters.set("L1", l1);
            if (isVerbose()) std::cout << "* Compiling..." << std::endl;
            robot.compile();
            if (isVerbose()) std::cout << "* Computing dynamics..." << std::endl;
            robot.computeDynamics();
            // robot.printDynamics();
            if (isVerbose()) std::cout << "* Publishing the robot..." << std::endl;
            server.loadRobot(&robot);

            if (isVerbose()) std::cout << "Initializing the controller..." << std::endl;
            Controller controller;
            controller.dx = 60;
            controller.freq = 2.0;

            Simulation simulation(15.0, server, robot, controller);
            simulation.factor = factor;
            auto cost = simulation.run();

            auto state = robot.getState();
            auto score = sqrt(state.x()*state.x() + state.y()*state.y() + state.z()*state.z());
            std::cout << l1 << " " << cost << " " << score << std::endl;
        }

    } catch (std::string err) {
        std::cerr << "Error: " << err << std::endl;
    }
}
