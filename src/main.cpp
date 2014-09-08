#include <string>
#include <iostream>
#include "Backend.h"

using namespace std;

int main()
{
    try {
        Metabot::Backend backend("xl320");
        backend.load();
    } catch (string error) {
        cerr << "[ERROR] " << error << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
