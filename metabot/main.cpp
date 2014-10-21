#include <string>
#include <iostream>
#include <3d/stl.h>
#include <getopt.h>
#include "util.h"
#include "Backend.h"
#include "AnchorPoint.h"
#include "ComponentInstance.h"
#include "Robot.h"
#include "util.h"

using namespace std;
using namespace Metabot;

Backend *backend = NULL;
Robot *robot = NULL;
string robotFile = "";

void usage()
{
    cerr << "Metabot v1.0 - Rhoban System" << endl;
    cerr << "Usage: metabot [option] [file.robot]" << endl;
    cerr << "" << endl;
    cerr << "   -c: Clears the cache" << endl;
    cerr << "   -w: Warmup/generates the cache" << endl;
    cerr << "   -s [output.stl]: Export the given robot to stl" << endl;
    exit(EXIT_FAILURE);
}

void needRobot()
{
    if (robotFile != "") {
        try {
            robot = new Robot(backend);
            robot->loadFromFile(robotFile);
        } catch (...) {
            cerr << "Error: unable to open " << robotFile << endl;
            exit(EXIT_FAILURE);
        }
    } else {
        cerr << "Error: no robot given" << endl;
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    int index;
    bool cacheClear = false;
    bool cacheWarmup = false;
    bool stlExport = false;
    string output;

    while ((index = getopt(argc, argv, "cws:")) != -1) {
        switch (index) {
            case 'c':
                cacheClear = true;
                break;
            case 'w':
                cacheWarmup = true;
                break;
            case 's':
                stlExport = true;
                output = string(optarg);
                break;
        }
    }

    if (optind != argc) {
        robotFile = string(argv[optind]);
    }

    try {
        // Loading the backend
        backend = new Backend("xl-320");
        backend->load();

        // Cache handling
        if (cacheClear) {
            cout << "Clearing the cache" << endl;
            int n = backend->clearCache();
            cout << "Done, removed " << n << " files." << endl;
        } else if (cacheWarmup) {
            cout << "Generating the cache..." << endl;
            backend->buildCache();
            cout << "Cache generated (" << backend->cacheFiles() << " files)." << endl;
        // Robot handling
        } else if (stlExport) {
            needRobot();
            cout << "Saving robot to " << output << endl;
            auto model = robot->toModel();
            saveModelToFileBinary(output.c_str(), &model);
        } else {
            usage();
        }

    } catch (string error) {
        cerr << "[ERROR] " << error << endl;
        return EXIT_FAILURE;
    }

    if (robot != NULL) {
        delete robot;
    }

    if (backend != NULL) {
        delete backend;
    }

    return EXIT_SUCCESS;
}
