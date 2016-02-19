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
{
}
                
void Simulator::Parameters::doNotOptimize(std::string name)
{
    notOptimized.insert(name);
}
                
void Simulator::Parameters::add(std::string name, double min, double max, double value, bool optimize)
{
    if (notOptimized.count(name)) optimize=false;
    Parameter param;
    param.name = name;
    param.value = value;
    param.min = min;
    param.max = max;
    param.optimize = optimize;
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
                
std::vector<std::string> Simulator::Parameters::getAll()
{
    std::vector<std::string> names;

    for (auto &entry : values) {
        names.push_back(entry.first);
    }

    return names;
}

std::vector<double> Simulator::Parameters::toVector()
{
    std::vector<double> vector;
    for (auto name : order) {
        if (values[name].optimize) {
            vector.push_back(values[name].normalize());
        }
    }
    return vector;
}

void Simulator::Parameters::fromArray(const double *x, const int N)
{
    int k = 0;
    for (auto name : order) {
        if (values[name].optimize) {
            values[name].fromNormalized(x[k++]);
            values[name].check();
        }
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

Simulator::Simulator(std::string robotFile, double factor, bool runServer, double dt)
    : serverThread(0), robotFile(robotFile), factor(factor), server(NULL), dt(dt)
{
    if (runServer) {
        server = new Metabot::Server;
    }
}

Simulator::~Simulator()
{
    if (server) {
        delete server;
    }
}
        
double Simulator::run(Parameters &parameters, double duration)
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
    //
    if (isVerbose()) std::cout << "* Publishing the robot..." << std::endl;
    if (serv) serv->loadRobot(&robot);

    if (isVerbose()) std::cout << "Initializing the controller..." << std::endl;
    Controller controller(&robot);
    controller.x = parameters.get("x");
    controller.y = parameters.get("y");
    controller.z = parameters.get("z");
    controller.freq = parameters.get("freq");
    controller.lX = parameters.get("lX");
    controller.lS = parameters.get("lS");
    controller.lH = parameters.get("lH");
    controller.support = parameters.get("support");
    controller.dx = parameters.get("dx");
    controller.dy = parameters.get("dy");
    controller.turn = parameters.get("turn");
    controller.setupFunctions();

    for (int k=1; k<=robot.tips(); k++) {
        std::stringstream p;
        p << "p" << k;
        controller.phases[k-1] = parameters.get(p.str());
    }

    // Creating the simulation
    Simulation simulation(duration, serv, robot, controller, dt);
    simulation.factor = factor;
    auto cost = simulation.run();
    auto state = robot.getState();

    return cost/(duration*fabs(state.x()));
}
