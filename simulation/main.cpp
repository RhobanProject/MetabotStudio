#include <math.h>
#include <getopt.h>
#include <sstream>
#include <deque>
#include <map>
#include <mutex>
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
    std::cout << "  -e: enable external mode for simulation" << std::endl;
    std::cout << "  -N: no server mode" << std::endl;
    exit(1);
}

int main(int argc, char *argv[])
{
    float factor = 100.0;
    float duration = 6.0;
    int index;
    bool external = false;
    bool noServer = false;
    std::string robotFile = "";
    std::string mode = "sim";

    while ((index = getopt(argc, argv, "r:vtf:d:ceN")) != -1) {
        switch (index) {
            case 'N':
                noServer = true;
                break;
            case 'e':
                external = true;
                noServer = true; 
                break;
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
        parameters.add("L1", 20, 250, 50, false);
        parameters.add("L2", 50, 250, 70, false);
        parameters.add("L3", 50, 250, 70, false);

        parameters.add("x", 0, 200, 120, false);
        parameters.add("y", 0, 200, 120, false);
        parameters.add("z", 0, 150, 50, false);
        
        parameters.add("freq", 0, 5, 2, false);
        parameters.add("alt", 0, 100, 18, false);
        parameters.add("dx", 0, 300, 60, false);
        
        parameters.add("p1", 0, 1, 0.5);
        parameters.add("p2", 0, 1, 0.0);
        parameters.add("p3", 0, 1, 0.5);
        parameters.add("p4", 0, 1, 0.0);
        
        parameters.add("friction", 0, 1, 0.5, false);
        parameters.add("maxSpeed", 0, 100, 4*M_PI, false);
        parameters.add("maxTorque", 0, 100, 0.5, false);
        
        parameters.add("dt", 0, 1, 0.001, false);
    
        for (int k=optind; k<argc; k++) {
            std::string value(argv[k]);
            auto parts = split(value, '=');
            if (parts.size() == 2) {
                parameters.set(parts[0], atof(parts[1].c_str()));
            } else {
                parameters.push(atof(argv[k]));
            }
        }
        Simulator simulator(robotFile, factor, !noServer, parameters.get("dt"));

        if (mode == "sim") {
            std::cout << "score=" << simulator.run(parameters, duration) << std::endl;
        }

        if (mode == "cmaes") {
            double start = getTime();

            // CMAES parameters
            CMAParameters<> cmaparams(parameters.toVector(), 0.5, 48);
            cmaparams.set_algo(BIPOP_CMAES);
            cmaparams.set_quiet(false);
            //cmaparams.set_max_iter(100);
            cmaparams.set_restarts(2);
            //cmaparams.set_max_hist(3);
            //cmaparams.set_max_fevals(10000000);
            cmaparams.set_elitism(1);
            //cmaparams.set_ftolerance(0.00001);
            cmaparams.set_mt_feval(true);
            cmaparams.set_ftarget(0.0);
            
            FitFunc robotSim = [robotFile, external, &parameters, &simulator, duration](const double *x, const int N)
            {
                Simulator::Parameters params = parameters;
                
                try {
                    params.fromArray(x, N);
                } catch (Simulator::ParameterError err) {
                    return err.error;
                }
                std::cout << params.toString() << std::endl;

                if (external) {
                    std::stringstream ss;
                    ss << "./sim -N -d " << duration << " -r " << robotFile << " " << params.toString();
                    auto result = execute(ss.str());
                    auto parts = split(result, '=');
                    if (parts.size() == 2 && parts[0] == "score") {
                        return atof(parts[1].c_str());
                    } else {
                        return 9999.0;
                    }
                }
                
                return simulator.run(params, duration);
            };

            CMASolutions cmasols = cmaes<>(robotSim, cmaparams);
            std::cout << "*** OVER" << std::endl;
            std::cout << cmasols << std::endl;
            auto best = cmasols.get_best_seen_candidate();
            parameters.fromArray(best.get_x_ptr(), best.get_x_size());
            std::cout << parameters.toString() << std::endl;
            std::cout << "Time: " << getTime()-start << std::endl;
        }
    } catch (std::string err) {
        std::cerr << "Error: " << err << std::endl;
    }

    Backend::clean();
}
