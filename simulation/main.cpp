#include <getopt.h>
#include <sstream>
#include <deque>
#include <map>
#include <unistd.h>
#include <Robot.h>
#include <Backend.h>
#include "Controller.h"
#include "GazeboRobot.h"
#include "util.h"
#include "verbose.h"

#include <iostream>

std::string joints[12] = {
    "joint_2",
    "joint_3",
    "joint_4",
    "joint_5",
    "joint_6",
    "joint_7",
    "joint_8",
    "joint_9",
    "joint_10",
    "joint_11",
    "joint_12",
    "joint_13",
};

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

    try { 
        if (isVerbose()) std::cout << "Connecting to Gazebo..." << std::endl;
        gazebo::client::setup(argc, argv);

        // Loading the robot
        if (isVerbose()) std::cout << "Loading the robot..." << std::endl;
        GazeboRobot metabot("metabot");

        Metabot::Robot robot;
        if (isVerbose()) std::cout << "* Reading file..." << std::endl;
        robot.loadFromFile(robotFile);

        for (double dx=10; dx<80; dx+=10) {
            if (isVerbose()) std::cout << "* Compiling..." << std::endl;
            robot.compile();
            if (isVerbose()) std::cout << "* Loading it to Gazebo..." << std::endl;
            metabot.load(robot);

            // Initializing the controller
            if (isVerbose()) std::cout << "Initializing the controller..." << std::endl;
            Controller controller;
            controller.dx = dx;
            controller.freq = 1.2;

            float t = 0.0;
            if (isVerbose()) std::cout << "Starting simulation..." << std::endl;
            while (t < 10) {
                metabot.tick(0.02);
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

        gazebo::client::shutdown();
    } catch (std::string err) {
        std::cerr << "Error: " << err << std::endl;
    }
}
