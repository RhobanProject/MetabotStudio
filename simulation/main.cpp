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
#include "Generator.h"

#include <iostream>

using namespace libcmaes;

static void usage()
{
    std::cout << "Metabot simulation (w/ Bullet)" << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "  -r [file.robot]: The robot file to simulate" << std::endl;
    std::cout << "  -v: verbose" << std::endl;
    std::cout << "  -f [factor]: set time factor" << std::endl;
    std::cout << "  -t: real time mode (like -f 1)" << std::endl;
    std::cout << "  -c: run CMA-ES optimisation" << std::endl;
    std::cout << "  -d [duration]: simulation duration" << std::endl;
    std::cout << "  -e: enable external mode for simulation" << std::endl;
    std::cout << "  -N: no server mode" << std::endl;
    std::cout << "  -G: generate a robot" << std::endl;
    std::cout << "  -x [exp]: specify experience" << std::endl;
    exit(1);
}

int main(int argc, char *argv[])
{
    float factor = 100.0;
    float duration = 0.0;
    int index;
    int experience = 1;
    bool external = false;
    bool noServer = false;
    std::string robotFile = "";
    std::string mode = "sim";

    while ((index = getopt(argc, argv, "r:vtf:d:ceNGx:")) != -1) {
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
            case 'G':
                mode = "generate";
                break;
            case 'x':
                experience = atoi(optarg);
                break;
        }
    }

    // Default durations
    if (duration == 0.0) {
        if (experience == 1) duration = 6;
        if (experience == 2) duration = 15;
    }

    if (mode == "generate") {
        auto backend = Backend::get("xl-320");
        Generator generator(backend);
        auto robot = generator.generate();
        auto json = robot->toJson();
        Json::StyledWriter writer;
        std::cout << writer.write(json) << std::endl;
        delete robot;

        return 0;
    }

    if (robotFile == "") {
        usage();
    }

    try { 
        Simulator::Parameters parameters;
        
        // This tells to the parameters that given names should not be
        // optimized
        for (int k=optind; k<argc; k++) {
            std::string value(argv[k]);
            auto parts = split(value, '=');
            if (parts.size() == 2) {
                parameters.doNotOptimize(parts[0]);
            }
        }

        // Robot parameters
        Metabot::Robot robot;
        robot.loadParametersFromFile(robotFile);
        for (auto entry : robot.parameters) {
            auto param = entry.second;
            parameters.add(param.name, param.getMin(), param.getMax(), param.getNumericValue());
        }
        
        // Posture parameters
        parameters.add("x", 0, 3, 0.8);
        parameters.add("y", 0, 3, 0.8);
        parameters.add("z", 0, 150, 50);

        // Controller parameters
        parameters.add("freq", 0, 4, 2);
        parameters.add("lX", -1, 1, 0);
        parameters.add("lH", 0, 100, 20);
        parameters.add("lS", 0, 10, 1);
        parameters.add("support", 0, 1, 0.5);
        parameters.add("dx", 0, 300, 60);
        parameters.add("dy", 0, 300, 0, false);
        if (experience == 1) {
            parameters.add("turn", -3, 3, 0, false);
        } else {
            parameters.add("turn", -3, 3, 0.5, true);
        }

        // Leg phases
        for (int k=1; k<=robot.tips(); k++) {
            std::stringstream p;
            p << "p" << k;
            parameters.add(p.str(), 0, 1, k%2 ? 0 : 0.5, k>1);
        }

        parameters.add("friction", 0, 1, 0.5, false);
        parameters.add("maxSpeed", 0, 100, 4*M_PI, false);
        parameters.add("maxTorque", 0, 100, 0.5, false);

        parameters.add("dt", 0, 1, 0.001, false);

        for (int k=optind; k<argc; k++) {
            std::string value(argv[k]);
            auto parts = split(value, '=');
            if (parts.size() == 2) {
                parameters.set(parts[0], atof(parts[1].c_str()));
            }
        }
        Simulator simulator(robotFile, factor, !noServer, parameters.get("dt"), experience);

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

            FitFunc robotSim = [robotFile, external, &parameters, &simulator, duration, experience](const double *x, const int N)
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
                    ss << "./sim -x " << experience << " -N -d " << duration << " -r " << robotFile << " " << params.toString();
                    auto result = execute(ss.str());
                    auto parts = split(result, '=');
                    if (parts.size() == 2 && parts[0] == "score") {
                        return atof(parts[1].c_str());
                    } else {
                        return 1e9;
                    }
                }

                return simulator.run(params, duration);
            };

            ProgressFunc<CMAParameters<>,CMASolutions> progressFunc = [parameters](const CMAParameters<> &cmaparams, const CMASolutions &cmasols)
            {
                auto bestParameters = parameters;
                auto best = cmasols.get_best_seen_candidate();
                std::cout << "*** CURRENT BEST (score: " << best.get_fvalue() << ") " << std::endl;
                try {
                    bestParameters.fromArray(best.get_x_ptr(), best.get_x_size());
                    std::cout << bestParameters.toString() << std::endl;
                } catch (Simulator::ParameterError err) {
                    std::cout << "(out of range)" << std::endl;
                }

                return 0;
            };

            CMASolutions cmasols = cmaes<>(robotSim, cmaparams, progressFunc);
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
