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
#include "util/util.h"
#include "verbose.h"
#include <cmaes.h>
#include "Simulator.h"

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
    std::cout << "  -v: verbose" << std::endl;
    std::cout << "  -f [factor]: set time factor" << std::endl;
    std::cout << "  -t: real time mode (like -f 1)" << std::endl;
    std::cout << "  -c: run CMA-ES optimisation" << std::endl;
    std::cout << "  -d [duration]: simulation duration" << std::endl;
    exit(1);
}

int main(int argc, char *argv[])
{
    float factor = 100.0;
    float duration = 6.0;
    int index;
    std::string robotFile = "";
    std::string mode = "sim";

    while ((index = getopt(argc, argv, "r:vtf:d:c")) != -1) {
        switch (index) {
            case 'd':
                duration = atof(optarg);
                break;
            case 'c':
                mode = "cmaes";
                break;
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

    try { 
        Simulator::Parameters parameters;
        parameters.add("L1", 20, 250, 32);
        parameters.add("L2", 50, 250, 54);
        parameters.add("L3", 50, 250, 92);

        parameters.add("r", 50, 150, 120);
        parameters.add("h", -150, 0, -50);
        
        parameters.add("freq", 0, 5, 2.0);
        parameters.add("alt", 0, 100, 27);
        parameters.add("dx", 0, 300, 68);
    
        for (int k=optind; k<argc; k++) {
            std::string value(argv[k]);
            auto parts = split(value, '=');
            if (parts.size() == 2) {
                parameters.set(parts[0], atof(parts[1].c_str()));
            } else {
                parameters.push(atof(argv[k]));
            }
        }
        Simulator simulator(robotFile, factor);

        if (mode == "sim") {
            simulator.run(parameters, duration);
        }

        if (mode == "cmaes") {
            // CMAES parameters
            CMAParameters<> cmaparams(parameters.toVector(), 10);
            cmaparams.set_algo(BIPOP_CMAES);
            cmaparams.set_quiet(false);
            cmaparams.set_max_iter(500);
            cmaparams.set_elitism(2);
            cmaparams.set_mt_feval(true);
            cmaparams.set_ftarget(0.0);
            
            FitFunc robotSim = [&parameters, &simulator, duration](const double *x, const int N)
            {
                Simulator::Parameters params = parameters;
                
                try {
                    params.fromArray(x, N);
                } catch (Simulator::ParameterError err) {
                    return err.error;
                }
                std::cout << params.toString() << std::endl;
                
                return simulator.run(params, duration);
            };

            CMASolutions cmasols = cmaes<>(robotSim, cmaparams);
            std::cout << "*** OVER" << std::endl;
            std::cout << cmasols << std::endl;
        }
    } catch (std::string err) {
        std::cerr << "Error: " << err << std::endl;
    }

    Backend::clean();
}
