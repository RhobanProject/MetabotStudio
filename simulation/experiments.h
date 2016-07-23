#pragma once

#include <map>
#include <string>
#include "Experiment.h"

Experiment::BaseRunner *getExperiment(std::string name);
std::map<std::string, Experiment::BaseRunner*> getExperiments();
