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

void usage()
{
    cerr << "Metabot v1.0 - Rhoban System" << endl;
    cerr << "" << endl;
    cerr << "   -c: Clears the cache" << endl;
    cerr << "   -w: Warmup/generates the cache" << endl;
}

int main(int argc, char *argv[])
{
    int index;
    bool cacheClear = false;
    bool cacheWarmup = false;

    while ((index = getopt(argc, argv, "cw")) != -1) {
        switch (index) {
            case 'c':
                cacheClear = true;
                break;
            case 'w':
                cacheWarmup = true;
                break;
        }
    }

    try {
        // Loading the backend
        Backend backend("xl-320");
        backend.load();

        // Cache warmup
        if (cacheClear) {
            cout << "Clearing the cache" << endl;
            int n = backend.clearCache();
            cout << "Done, removed " << n << " files." << endl;
        } else if (cacheWarmup) {
            cout << "Generating the cache..." << endl;
            backend.buildCache();
            cout << "Cache generated (" << backend.cacheFiles() << " files)." << endl;
        } else {
            usage();
        }

    } catch (string error) {
        cerr << "[ERROR] " << error << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
