#include <string>
#include <iostream>
#include "util.h"
#include "Backend.h"
#include "ComponentInstance.h"

using namespace std;
using namespace Metabot;

int main()
{
    try {
        Metabot::Backend backend("xl-320");
        backend.load();

        auto components = backend.getComponents("root");
        auto root = components[0]->instanciate();
        root->set("Legs", "3");
        root->compile();

    } catch (string error) {
        cerr << "[ERROR] " << error << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
