#include <iostream>
#include <sstream>
#include <math.h>
#include "verbose.h"
#include "Simulation.h"
#include "Experience.h"

Experience::ParameterError::ParameterError(double error)
    : error(error)
{
}

double Experience::Parameter::normalize()
{
    double delta = max-min;
    return (value-min)/delta;
}

void Experience::Parameter::fromNormalized(double n)
{
    double delta = max-min;
    value = min+n*delta;
}

void Experience::Parameter::check()
{
    if (value < min) throw ParameterError(1e7+fabs(value-min));
    if (value > max) throw ParameterError(1e7+fabs(value-max));
}

Experience::Parameters::Parameters()
{
}
                
void Experience::Parameters::doNotOptimize(std::string name)
{
    notOptimized.insert(name);
}
                
void Experience::Parameters::add(std::string name, double min, double max, double value, bool optimize)
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

void Experience::Parameters::set(std::string name, double value)
{
    if (values.count(name)) {
        values[name].value = value;
    }
}

void Experience::Parameters::check(std::string name)
{
    values[name].check();
}
                
double Experience::Parameters::get(std::string name)
{
    if (values.count(name)) {
        return values[name].value;
    }

    return 0.0;
}
                
std::vector<std::string> Experience::Parameters::getAll()
{
    std::vector<std::string> names;

    for (auto &entry : values) {
        names.push_back(entry.first);
    }

    return names;
}

std::vector<double> Experience::Parameters::toVector()
{
    std::vector<double> vector;
    for (auto name : order) {
        if (values[name].optimize) {
            vector.push_back(values[name].normalize());
        }
    }
    return vector;
}

void Experience::Parameters::fromArray(const double *x, const int N)
{
    int k = 0;
    for (auto name : order) {
        if (values[name].optimize) {
            values[name].fromNormalized(x[k++]);
            values[name].check();
        }
    }
}
                
std::string Experience::Parameters::toString()
{
    std::stringstream ss;
    for (auto name : order) {
        auto param = values[name];
        ss << param.name << "=" << param.value << " ";
    }
    return ss.str();
}

void Experience::init(Experience::Parameters &parameters, Metabot::Robot *robot)
{
}

void Experience::control(Simulation *simulation)
{
}
        
double Experience::score(Simulation *simulation)
{
    return 0.0;
} 
        
void Experience::initParameters(Parameters &parameters, Metabot::Robot *robot)
{
}
        
double Experience::defaultDuration()
{
    return 6.0;
}
        
double Experience::setMotorTargets(Simulation *simulation, std::vector<double> angles)
{
    int k = 0;
    double cost = 0;
    simulation->robot.foreachComponent([simulation, &cost, &k, &angles](Metabot::Component *component) {
        if (component->hinge) {
            cost += component->setTarget(angles[k++], simulation->dt);
        }
    });

    return cost;
}
