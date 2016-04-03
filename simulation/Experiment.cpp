#include <iostream>
#include <sstream>
#include <math.h>
#include "verbose.h"
#include "Simulation.h"
#include "Controller.h"
#include "Experiment.h"
#include "util.h"

Experiment::ParameterError::ParameterError(double error)
    : error(error)
{
}

double Experiment::Parameter::normalize()
{
    double delta = max-min;
    return (value-min)/delta;
}

void Experiment::Parameter::fromNormalized(double n)
{
    double delta = max-min;
    value = min+n*delta;
}

void Experiment::Parameter::check()
{
    if (value < min) throw ParameterError(1e10+fabs(value-min));
    if (value > max) throw ParameterError(1e10+fabs(value-max));
}

void Experiment::Parameter::randomize()
{
    double delta = max-min;
    value = min+randDouble()*delta;
}

Experiment::Parameters::Parameters()
{
}
                
void Experiment::Parameters::doNotOptimize(std::string name)
{
    notOptimized.insert(name);
}
                
void Experiment::Parameters::add(std::string name, double min, double max, double value, bool optimize)
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

void Experiment::Parameters::set(std::string name, double value)
{
    if (values.count(name)) {
        values[name].value = value;
    }
}
                
void Experiment::Parameters::randomize(std::string name)
{
    if (values.count(name)) {
        values[name].randomize();
    }
}

void Experiment::Parameters::check(std::string name)
{
    values[name].check();
}
                
double Experiment::Parameters::get(std::string name)
{
    if (values.count(name)) {
        return values[name].value;
    }

    return 0.0;
}
                
std::vector<std::string> Experiment::Parameters::getAll()
{
    std::vector<std::string> names;

    for (auto &entry : values) {
        names.push_back(entry.first);
    }

    return names;
}

std::vector<double> Experiment::Parameters::toVector()
{
    std::vector<double> vector;
    for (auto name : order) {
        if (values[name].optimize) {
            vector.push_back(values[name].normalize());
        }
    }
    return vector;
}

void Experiment::Parameters::fromArray(const double *x, const int N)
{
    int k = 0;
    for (auto name : order) {
        if (values[name].optimize) {
            values[name].fromNormalized(x[k++]);
            values[name].check();
        }
    }
}
                
std::string Experiment::Parameters::toString()
{
    std::stringstream ss;
    for (auto name : order) {
        auto param = values[name];
        ss << param.name << "=" << param.value << " ";
    }
    return ss.str();
}

void Experiment::init(Experiment::Parameters &parameters, Metabot::Robot *robot)
{
}

bool Experiment::end(Simulation *simulation)
{
    return true;
}

void Experiment::control(Simulation *simulation)
{
}
        
double Experiment::score(Simulation *simulation)
{
    return 0.0;
} 
        
void Experiment::initParameters(Parameters &parameters, Metabot::Robot *robot)
{
}
        
double Experiment::defaultDuration()
{
    return 6.0;
}
