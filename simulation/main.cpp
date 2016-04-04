#include <math.h>
#include <getopt.h>
#include <sstream>
#include <deque>
#include <thread>
#include <mutex>
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
#include "Experiment.h"
#include "ExperimentTest.h"
#include "ExperimentWalk.h"
#include "ExperimentCheckpoints.h"
#include "ExperimentStandUp.h"
#include "ExperimentShoot.h"
#include "ExperimentStaticShoot.h"
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
    std::cout << "  -x [exp]: specify experiment" << std::endl;
    exit(1);
}

int main(int argc, char *argv[])
{
    float factor = 100.0;
    float duration = 0.0;
    int index;
    bool external = false;
    bool noServer = false;
    std::string robotFile = "";
    std::string mode = "sim";
    std::string experiment = "walk";
    std::cout.precision(16);

    while ((index = getopt(argc, argv, "r:vtf:d:ceNGx:b")) != -1) {
        switch (index) {
            case 'b':
                mode = "brute";
                break;
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
                experiment = std::string(optarg);
                break;
        }
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
        // Creating the experiment
        Experiment::BaseRunner *runner = NULL;
        if (experiment == "walk") {
            runner = new Experiment::Runner<ExperimentWalkEfficience>();
        } else if (experiment == "walk-speed") {
            runner = new Experiment::Runner<ExperimentWalkSpeed>();
        } else if (experiment == "checkpoints") {
            runner = new Experiment::Runner<ExperimentCheckpointsEfficience>();
        } else if (experiment == "checkpoints-speed") {
            runner = new Experiment::Runner<ExperimentCheckpointsSpeed>();
        } else if (experiment == "none") {
            runner = new Experiment::Runner<Experiment>();
        } else if (experiment == "zero") {
            runner = new Experiment::Runner<ExperimentZero>();
        } else if (experiment == "sinus") {
            runner = new Experiment::Runner<ExperimentSinus>();
        } else if (experiment == "standup") {
            runner = new Experiment::Runner<ExperimentStandUp>();
        } else if (experiment == "shoot") {
            runner = new Experiment::Runner<ExperimentShoot>();
        } else if (experiment == "staticshoot") {
            runner = new Experiment::Runner<ExperimentStaticShoot>();
        }

        runner->init(robotFile, factor, !noServer, 0.001);

        // Reading parameters
        Experiment::Parameters parameters;
        
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
        
        // Experiment parameters
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
                } catch (Experiment::ParameterError error) {
                    printf("score=%lf\n", error.error);
                    exit(0);
                }
            }
        }

        if (mode == "sim") {
            printf("score=%lf\n", runner->run(parameters, duration));
        }

        if (mode == "brute") {
            auto evaluate = [robotFile, external, duration, experiment](Experiment::Parameters parameters) {
                std::stringstream ss;
                ss << "./sim -x " << experiment << " -N -d " << duration << " -r " << robotFile 
                    << " " << parameters.toString();
                auto result = execute(ss.str());
                auto parts = split(result, '=');
                if (parts.size() == 2 && parts[0] == "score") {
                    return atof(parts[1].c_str());
                } else {
                    return 1e20;
                }
            };

            std::mutex mutex;
            std::vector<Experiment::Parameters> todo;
            auto getNext = [&parameters, &mutex, &todo]() {
                Experiment::Parameters task;
                bool over = false;

                mutex.lock();
                if (todo.size()) {
                    task = todo.back();
                    todo.pop_back();
                } else {
                    over = true;
                }
                // Update params
                mutex.unlock();

                if (over) {
                    throw std::string("over");
                }

                return task;
            };

            for (float p2=0; p2<1; p2+=0.025) {
                parameters.set("p2", p2);
                for (float p3=0; p3<1; p3+=0.025) {
                    parameters.set("p3", p3);
                    for (float p4=0; p4<1; p4+=0.025) {
                        parameters.set("p4", p4);
                        todo.push_back(parameters);
                    }
                }
            }

            std::vector<std::thread*> threads;
            for (int t=0; t<48; t++) {
                std::thread *x = new std::thread([&evaluate, &getNext, &mutex]() {
                    try {
                        while (true) {
                            auto params = getNext();
                            auto result = evaluate(params);
                            mutex.lock();
                            std::cout << params.toString() << " score=" << result << std::endl;
                            mutex.unlock();
                        }
                    } catch (std::string over) {
                    }
                });
                threads.push_back(x);
            }
            for (auto x : threads) {
                x->join();
            }
        }

        if (mode == "cmaes") {
            double start = getTime();

            // CMAES parameters
            //CMAParameters<> cmaparams(parameters.toVector(), -1, 64);
            CMAParameters<> cmaparams(parameters.toVector(), -1, 64);
            cmaparams.set_algo(BIPOP_CMAES);
            cmaparams.set_quiet(false);
            //cmaparams.set_max_iter(100);
            cmaparams.set_restarts(3);
            //cmaparams.set_max_hist(3);
            //cmaparams.set_max_fevals(10000000);
            //cmaparams.set_elitism(0);
            //cmaparams.set_x0(0, 1);
            cmaparams.set_ftolerance(1e-6);
            cmaparams.set_xtolerance(1e-3);
            cmaparams.set_mt_feval(true);
            cmaparams.set_ftarget(0.0);

            FitFunc robotSim = [robotFile, external, &parameters, &runner, duration, experiment](const double *x, const int N)
            {
                Experiment::Parameters params = parameters;

                try {
                    params.fromArray(x, N);
                } catch (Experiment::ParameterError err) {
                    return err.error;
                }
                //std::cout << params.toString() << std::endl;

                if (external) {
                    std::stringstream ss;
                    ss << "./sim -x " << experiment << " -N -d " << duration << " -r " << robotFile << " " << params.toString();
                    // std::cout << ss.str() << std::endl;
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
                } catch (Experiment::ParameterError err) {
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
