#include <string>
#include <map>
#include "experiments.h"
#include "ExperimentTest.h"

#include "ExperimentWalk.h"
#include "ExperimentCheckpoints.h"

#include "ExperimentStandUp.h"
#include "ExperimentShoot.h"
#include "ExperimentStaticShoot.h"

static std::map<std::string, Experiment::BaseRunner*> mapping;
static bool initialized = false;

template<class T>
static void registerExperiment(std::string name)
{
    mapping[name] = new Experiment::Runner<T>();
}

static void initialize()
{
    if (!initialized) {
        initialized=true;

        // Test experiments
        registerExperiment<Experiment>("none");
        registerExperiment<ExperimentZero>("zero");
        registerExperiment<ExperimentSinus>("sinus");

        // Legged robot experiments
        registerExperiment<ExperimentWalkEfficience>("walk");
        registerExperiment<ExperimentWalkSpeed>("walk-speed");
        registerExperiment<ExperimentCheckpointsEfficience>("checkpoints");
        registerExperiment<ExperimentCheckpointsSpeed>("checkpoints-speed");

        // Humanoid experiments
        registerExperiment<ExperimentStandUp>("standup");
        registerExperiment<ExperimentShoot>("shoot");
        registerExperiment<ExperimentStaticShoot>("staticshoot");
    }
}

std::map<std::string, Experiment::BaseRunner*> getExperiments()
{
    initialize();
    return mapping;
}

Experiment::BaseRunner *getExperiment(std::string name)
{
    initialize();

    if (mapping.count(name)) {
        return mapping[name];
    } else {
        return NULL;
    }
}
