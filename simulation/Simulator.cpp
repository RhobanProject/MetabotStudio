#include <iostream>
#include <sstream>
#include <math.h>
#include "verbose.h"
#include "Simulation.h"
#include "Controller.h"
#include "Simulator.h"

Simulator::ParameterError::ParameterError(double error)
    : error(error)
{
}

double Simulator::Parameter::normalize()
{
    double delta = max-min;
    return (value-min)/delta;
}

void Simulator::Parameter::fromNormalized(double n)
{
    double delta = max-min;
    value = min+n*delta;
}

void Simulator::Parameter::check()
{
    if (value < min) throw ParameterError(1000+fabs(value-min));
    if (value > max) throw ParameterError(1000+fabs(value-max));
}

Simulator::Parameters::Parameters()
    : pushIndex(0)
{
}
                
void Simulator::Parameters::add(std::string name, double min, double max, double value)
{
    Parameter param;
    param.name = name;
    param.value = value;
    param.min = min;
    param.max = max;
    order.push_back(name);
    values[name] = param;
}

void Simulator::Parameters::set(std::string name, double value)
{
    if (values.count(name)) {
        values[name].value = value;
    }
}
                
double Simulator::Parameters::get(std::string name)
{
    if (values.count(name)) {
        return values[name].value;
    }

    return 0.0;
}

std::vector<double> Simulator::Parameters::toVector()
{
    std::vector<double> vector;
    for (auto name : order) {
        vector.push_back(values[name].normalize());
    }
    return vector;
}

void Simulator::Parameters::fromArray(const double *x, const int N)
{
    int k = 0;
    for (auto name : order) {
        values[name].fromNormalized(x[k++]);
        values[name].check();
    }
}
                
std::string Simulator::Parameters::toString()
{
    std::stringstream ss;
    for (auto name : order) {
        auto param = values[name];
        ss << param.name << "=" << param.value << " ";
    }
    return ss.str();
}

void Simulator::Parameters::push(double value)
{
    if (pushIndex < order.size()) {
        values[order[pushIndex]].value = value;
    } else {
        throw std::string("Too much parameters pushed");
    }
    pushIndex++;
}
        
Simulator::Simulator(std::string robotFile, double factor, bool runServer)
    : serverThread(0), robotFile(robotFile), factor(factor), server(NULL)
{
    if (runServer) {
        server = new Metabot::Server;
    }
}

Simulator::~Simulator()
{
    for (auto robot : robots) {
        delete robot.second;
    }
    if (server) {
        delete server;
    }
}
        
double Simulator::run(Parameters &parameters, double duration)
{
    auto id = pthread_self();
    Metabot::Robot *robot = NULL;
    Metabot::Server *serv = NULL;
    
    mutex.lock();
    // Checking for serverThread
    if (serverThread == 0) {
        serverThread = id;
    }
    if (serverThread == id) {
        serv = server;
    }

    // Getting this thread robot
    if (!robots.count(id)) {
        robots[id] = new Metabot::Robot;
        robots[id]->loadFromFile(robotFile);
    }
    robot = robots[id];
    mutex.unlock();

    // Setting parameters
    robot->parameters.set("L1", round(parameters.get("L1")));
    robot->parameters.set("L2", round(parameters.get("L2")));
    robot->parameters.set("L3", round(parameters.get("L3")));
    
    if (isVerbose()) std::cout << "* Compiling..." << std::endl;
    robot->compile();
    
    if (isVerbose()) std::cout << "* Computing dynamics..." << std::endl;
    robot->computeDynamics();
    // robot.printDynamics();
    //
    if (isVerbose()) std::cout << "* Publishing the robot..." << std::endl;
    if (serv) serv->loadRobot(robot);

    if (isVerbose()) std::cout << "Initializing the controller..." << std::endl;
    Controller controller;
    controller.r = parameters.get("r");
    controller.h = parameters.get("h");
    controller.freq = parameters.get("freq");
    controller.alt = parameters.get("alt");
    controller.dx = parameters.get("dx");

    // Creating the simulation
    Simulation simulation(duration, serv, *robot, controller);
    simulation.factor = factor;
    auto cost = simulation.run();
    auto state = robot->getState();

    return cost/(duration*fabs(state.x()));
}
