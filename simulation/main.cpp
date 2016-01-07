#include <math.h>
#include <getopt.h>
#include <sstream>
#include <deque>
#include <map>
#include <unistd.h>
#include <Robot.h>
#include <Backend.h>
#include <com/Server.h>
#include <pthread.h>
#include "Controller.h"
#include "util.h"
#include "verbose.h"
#include <cmaes.h>

#include <iostream>

#define PARAM_BOUND(p,low,high) \
    if (p<low) return (1000+fabs(p-low)); \
    if (p>high) return (1000+fabs(p-high)); \

using namespace libcmaes;

static void usage()
{
    std::cout << "Metabot simulation (w/ Gazebo)" << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "  -r [file.robot]: The robot file to simulate" << std::endl;
    exit(1);
}

int main(int argc, char *argv[])
{
    float factor = 100.0;
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

        // Parameters
        std::vector<double> parameters(8, 0.0);

        // Morphology
        parameters[0] = 60;     // L1
        parameters[1] = 70;     // L2
        parameters[2] = 70;     // L3
        
        // Position
        parameters[3] = 125;    // r
        parameters[4] = -55;    // h

        // Controller
        // parameters[5] = 1.0;    // freq
        parameters[5] = 5.0;    // freq
        parameters[6] = 15;     // alt
        parameters[7] = 65;     // dx

        // CMAES parameters
        CMAParameters<> cmaparams(parameters, 10);
        cmaparams.set_algo(BIPOP_CMAES);
        cmaparams.set_quiet(false);
        cmaparams.set_max_iter(500);
        cmaparams.set_elitism(2);
        cmaparams.set_mt_feval(true);
        cmaparams.set_ftarget(0.0);

        pthread_t serverThread = 0;
           
        FitFunc robotSim = [robotFile, factor, &server, &serverThread](const double *x, const int N)
        {
            auto id = pthread_self();
            if (serverThread == 0) {
                serverThread = id;
            }

            // Loading the robot
            Metabot::Robot robot;
            if (isVerbose()) std::cout << "* Reading file..." << std::endl;
            robot.loadFromFile(robotFile);

            printf("[%lu] L1=%g, L2=%g, L3=%g, r=%g, h=%g, freq=%g, alt=%g, dx=%g\n", pthread_self(), 
                    x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7]);
            PARAM_BOUND(x[0], 20, 250);
            PARAM_BOUND(x[1], 50, 250);
            PARAM_BOUND(x[2], 50, 250);
            
            PARAM_BOUND(x[3], 50, 250);
            PARAM_BOUND(x[4], -150, 0);

            PARAM_BOUND(x[5], 0, 5);
            PARAM_BOUND(x[6], 0, 50);
            PARAM_BOUND(x[7], 0, 300);

            robot.parameters.set("L1", round(x[0]));
            robot.parameters.set("L2", round(x[1]));
            robot.parameters.set("L3", round(x[2]));
            if (isVerbose()) std::cout << "* Compiling..." << std::endl;
            robot.compile();
            if (isVerbose()) std::cout << "* Computing dynamics..." << std::endl;
            robot.computeDynamics();
            // robot.printDynamics();
            if (isVerbose()) std::cout << "* Publishing the robot..." << std::endl;
            if (id == serverThread) server.loadRobot(&robot);

            if (isVerbose()) std::cout << "Initializing the controller..." << std::endl;
            Controller controller;
            controller.r = x[3];
            controller.h = x[4];
            controller.freq = x[5];
            controller.alt = x[6];
            controller.dx = x[7];

            Simulation simulation(10.0, id==serverThread ? &server : NULL, robot, controller);
            simulation.factor = factor;
            auto cost = simulation.run();

            auto state = robot.getState();
            // auto score = sqrt(state.x()*state.x() + state.y()*state.y() + state.z()*state.z());

            return cost/fabs(state.x());
        };

        CMASolutions cmasols = cmaes<>(robotSim, cmaparams);
        std::cout << "~ OVER" << std::endl;
        std::cout << cmasols << std::endl;
    } catch (std::string err) {
        std::cerr << "Error: " << err << std::endl;
    }

    Backend::clean();
}
