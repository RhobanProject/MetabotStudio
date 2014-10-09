#include <string>
#include <iostream>
#include <3d/stl.h>
#include "util.h"
#include "Backend.h"
#include "AnchorPoint.h"
#include "ComponentInstance.h"
#include "Robot.h"
#include "util.h"

using namespace std;
using namespace Metabot;

int main()
{
    try {
        // Loading the backend
        Backend backend("xl-320");
        backend.load();
        backend.buildCache();

        // Making a robot
        Robot *robot = new Robot(&backend);

        /*
        robot->root = backend.getComponent("body")->instanciate();
        robot->root->set("Size", "35");
        robot->root->set("Legs", "4");
        robot->root->compile();

        for (int i=0; i<4; i++) {
            ComponentInstance *double_u = backend.getComponent("double_u")->instanciate();
            double_u->compile();
            robot->root->anchors[i]->attach(double_u->anchors[0]);

            ComponentInstance *side = backend.getComponent("side_to_side")->instanciate();
            side->compile();
            double_u->anchors[1]->attach(side->anchors[0]);
            double_u->anchors[1]->alpha = -DEG2RAD(30);

            ComponentInstance *leg = backend.getComponent("arm_leg")->instanciate();
            leg->compile();
            leg->set("SizeB", "40");
            leg->set("SizeC", "40");
            side->anchors[1]->attach(leg->anchors[0]);
            side->anchors[1]->alpha = DEG2RAD(110);
        }
        */

        robot->loadFromFile("/home/gregwar/Metabot/robots/spidey12.robot");
       
        // Getting 3D model
        // Model model = robot->toModel();
        // saveModelToFileBinary("/tmp/demo.stl", &model);
        
        // Running the build
        robot->build("/tmp/metabot");

    } catch (string error) {
        cerr << "[ERROR] " << error << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
