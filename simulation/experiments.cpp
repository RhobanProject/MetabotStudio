#include <string>
#include <map>
#include "experiments.h"
#include "ExperimentTest.h"

#ifdef ENABLE_LEGGED
#include "legged/ExperimentWalk.h"
#include "legged//ExperimentCheckpoints.h"
#include "legged//ExperimentAnalyze.h"
#endif

#ifdef ENABLE_HUMANOID
#include "humanoid/ExperimentStandUp.h"
#include "humanoid/ExperimentShoot.h"
#include "humanoid/ExperimentStaticShoot.h"
#endif

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

#ifdef ENABLE_LEGGED
        // Legged robot experiments
        registerExperiment<ExperimentWalkEfficience>("walk");
        registerExperiment<ExperimentWalkSpeed>("walk-speed");
        registerExperiment<ExperimentCheckpointsEfficience>("checkpoints");
        registerExperiment<ExperimentCheckpointsSpeed>("checkpoints-speed");
        registerExperiment<ExperimentAnalyze>("analyze");
#endif

#ifdef ENABLE_HUMANOID
        // Humanoid experiments
        registerExperiment<ExperimentStandUp>("standup");
        registerExperiment<ExperimentShoot>("shoot");
        registerExperiment<ExperimentStaticShoot>("staticshoot");
#endif
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
