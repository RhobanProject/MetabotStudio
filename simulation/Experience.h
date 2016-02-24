#pragma once

#include <set>
#include <mutex>
#include <string>
#include <map>
#include <vector>
#include <Robot.h>
#include <Backend.h>
#include <com/Server.h>
#include "Simulation.h"
#include "verbose.h"

class Experience
{
    public:
        class ParameterError
        {
            public:
                ParameterError(double error);

                double error;
        };

        class Parameter
        {
            public:
                void check();
                double normalize();
                void fromNormalized(double n);

                std::string name;
                double value;
                double min;
                double max;
                bool optimize;
        };

        class Parameters
        {
            public:
                Parameters();
                void doNotOptimize(std::string name);
                void add(std::string name, double min, double max, double value, bool optimize=true);
                void set(std::string name, double value);
                void check(std::string name);
                double get(std::string name);

                std::vector<double> toVector();
                std::vector<std::string> getAll();
                void fromArray(const double *x, const int N);
                std::string toString();
                void push(double value);

            protected:
                int pushIndex;
                std::vector<std::string> order;
                std::set<std::string> notOptimized;
                std::map<std::string, Parameter> values;
        };

        class BaseRunner
        {
            public:
                virtual void init(std::string robotFile_, double factor_, bool runServer, double dt_)=0;
                virtual void initParameters(Parameters &parameters, Metabot::Robot *robot)=0;
                virtual double run(Parameters &parameters, double duration=6.0)=0;
        };

        template<typename T>
            class Runner : public BaseRunner
        {
            public:
                void init(std::string robotFile_, double factor_, bool runServer, double dt_)
                {
                    robotFile = robotFile_;
                    factor = factor_;
                    dt = dt_;
                    
                    if (runServer) {
                        server = new Metabot::Server;
                    }
                }

                ~Runner()
                {
                    if (server) {
                        delete server;
                    }
                }

                void initParameters(Parameters &parameters, Metabot::Robot *robot)
                {
                    T::initParameters(parameters, robot);
                }

                double run(Parameters &parameters, double duration=6.0)
                {
                    auto id = pthread_self();
                    Metabot::Server *serv = NULL;

                    mutex.lock();
                    // Checking for serverThread
                    if (serverThread == 0) {
                        serverThread = id;
                    }
                    if (serverThread == id) {
                        serv = server;
                    }
                    mutex.unlock();

                    Metabot::Parameters defines;
                    Metabot::Robot robot;

                    // Setting parameters
                    for (auto name : parameters.getAll()) {
                        defines.set(name, round(parameters.get(name)));
                    }
                    robot.world.friction = parameters.get("friction");
                    robot.loadFromFile(robotFile, defines);

                    if (isVerbose()) std::cout << "* Computing dynamics..." << std::endl;
                    robot.computeDynamics();
                    robot.setMotorsLimit(parameters.get("maxSpeed"), parameters.get("maxTorque"));
                    // robot.printDynamics();

                    if (isVerbose()) std::cout << "* Publishing the robot..." << std::endl;
                    if (serv) serv->loadRobot(&robot);
                    if (serv) serv->disableMarker();

                    // Creating the experience
                    T experience;
                    experience.init(parameters, &robot);

                    if (duration == 0.0) {
                        duration = T::defaultDuration();
                    }

                    // Creating the simulation
                    Simulation simulation(duration, serv, robot, dt);
                    simulation.factor = factor;
                    simulation.run([&experience](Simulation *simulation) {
                        experience.control(simulation);
                    });

                    return experience.score(&simulation);
                }

                Metabot::Server *server;
                std::string robotFile;
                double factor;
                double dt;
                std::mutex mutex;
                pthread_t serverThread;
        };

        // Called on experience initialization
        virtual void init(Parameters &parameters, Metabot::Robot *robot);

        // Called on each tick of the experience
        virtual void control(Simulation *simulation);

        // Called at the end to compute the score
        virtual double score(Simulation *simulation);

        // Initialize parameters
        static void initParameters(Parameters &parameters, Metabot::Robot *robot);

        // Default experience duration
        static double defaultDuration();
};
