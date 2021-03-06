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
#include "util.h"
#include "util/util.h"
#include "verbose.h"
#include <cmaes.h>
#include "Experience.h"
#include "ExperienceTest.h"
#include "ExperienceWalk.h"

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
    std::cout << "  -x [exp]: specify experience" << std::endl;
    exit(1);
}

int main(int argc, char *argv[])
{
    float factor = 1.0;
    float duration = 0.0;
    int index;
    bool external = false;
    bool noServer = false;
    std::string robotFile = "";
    std::string mode = "sim";
    std::string experience = "walk";

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
            case 'x':
                experience = std::string(optarg);
                break;
        }
    }

    if (robotFile == "") {
        usage();
    }

    try {
        // Creating the experience
        Experience::BaseRunner *runner = NULL;
        if (experience == "walk") {
            runner = new Experience::Runner<ExperienceWalk>();
        } else if (experience == "none") {
            runner = new Experience::Runner<Experience>();
        } else if (experience == "zero") {
            runner = new Experience::Runner<ExperienceZero>();
        } else if (experience == "sinus") {
            runner = new Experience::Runner<ExperienceSinus>();
        }

        runner->init(robotFile, factor, !noServer, 0.001);

        // Reading parameters
        Experience::Parameters parameters;
        
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
            parameters.add(param.name, param.getMin(), param.getMax(), param.getNumericValue(), false);
        }
        
        // Experience parameters
        runner->initParameters(parameters, &robot);

        // Context parameters
        parameters.add("friction", 0, 1, 0.5, false);

        // Reading parameters from command line
        for (int k=optind; k<argc; k++) {
            std::string value(argv[k]);
            auto parts = split(value, '=');
            if (parts.size() == 2) {
                try {
                    parameters.set(parts[0], atof(parts[1].c_str()));
                    parameters.check(parts[0]);
                } catch (Experience::ParameterError error) {
                    printf("score=%lf\n", error.error);
                    exit(0);
                }
            }
        }

        if (mode == "sim") {
            printf("score=%lf\n", runner->run(parameters, duration));
        }

        if (mode == "cmaes") {
            double start = getTime();

            // CMAES parameters
            CMAParameters<> cmaparams(parameters.toVector(), -1, 64);
            cmaparams.set_algo(BIPOP_CMAES);
            cmaparams.set_quiet(false);
            //cmaparams.set_max_iter(100);
            cmaparams.set_restarts(3);
            //cmaparams.set_max_hist(3);
            //cmaparams.set_max_fevals(10000000);
            cmaparams.set_elitism(2);
            cmaparams.set_x0(0, 1);
            //cmaparams.set_ftolerance(0.00001);
            cmaparams.set_mt_feval(true);
            cmaparams.set_ftarget(0.0);

            FitFunc robotSim = [robotFile, external, &parameters, &runner, duration, experience](const double *x, const int N)
            {
                Experience::Parameters params = parameters;

                try {
                    params.fromArray(x, N);
                } catch (Experience::ParameterError err) {
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

                return runner->run(params, duration);
            };

            ProgressFunc<CMAParameters<>,CMASolutions> progressFunc = [parameters](const CMAParameters<> &cmaparams, const CMASolutions &cmasols)
            {
                auto bestParameters = parameters;
                auto best = cmasols.get_best_seen_candidate();
                std::cout << "*** CURRENT BEST (score: " << best.get_fvalue() << ") " << std::endl;
                try {
                    bestParameters.fromArray(best.get_x_ptr(), best.get_x_size());
                    std::cout << bestParameters.toString() << std::endl;
                } catch (Experience::ParameterError err) {
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
