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
    cerr << "Cache operations" << endl;
    cerr << "   -c: Clears the cache" << endl;
    cerr << "   -w: Warmup/generates the cache" << endl;
    cerr << "" << endl;
    cerr << "Robot operations (needs a .robot)" << endl;
    cerr << "   -s [output.stl]: Export the given robot to stl" << endl;
    cerr << "   -p: shows the list of parts to print (stl and quantity)" << endl;
    cerr << "   -b: shows the BOM" << endl;
    exit(EXIT_FAILURE);
}

void needRobot()
{
    if (robotFile != "") {
        try {
            robot = new Robot(backend);
            robot->loadFromFile(robotFile);
        } catch (string err) {
            cerr << "Error: unable to open " << robotFile << " (" << err << ")" << endl;
            exit(EXIT_FAILURE);
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
    string mode = "";
    string output;

    while ((index = getopt(argc, argv, "pbcws:")) != -1) {
        switch (index) {
            case 'c':
                mode = "cacheClear";
                break;
            case 'w':
                mode = "cacheWarmup";
                break;
            case 's':
                mode = "stlExport";
                output = string(optarg);
                break;
            case 'b':
                mode = "bom";
                break;
            case 'p':
                mode = "parts";
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
        if (mode == "cacheClear") {
            cout << "Clearing the cache" << endl;
            int n = backend->clearCache();
            cout << "Done, removed " << n << " files." << endl;
        } else if (mode == "cacheWarmup") {
            cout << "Generating the cache..." << endl;
            backend->buildCache();
            cout << "Cache generated (" << backend->cacheFiles() << " files)." << endl;
        // Robot handling
        } else if (mode == "stlExport") {
            needRobot();
            cout << "Saving robot to " << output << endl;
            auto model = robot->toModel();
            saveModelToFileBinary(output.c_str(), &model);
        } else if (mode == "bom") {
            needRobot();
            cout << "Bill of materials" << endl;
            cout << robot->getBOM().toString();
        } else if (mode == "parts") {
            needRobot();
            cout << "Parts" << endl;
            cout << robot->getParts().group().toString();
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
