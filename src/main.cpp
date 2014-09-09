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

        // Making a robot
        Robot robot;
        robot.root = backend.getComponent("body")->instanciate();
        robot.root->set("Legs", "4");
        robot.root->compile();

        for (int i=0; i<4; i++) {
            ComponentInstance *double_u = backend.getComponent("double_u")->instanciate();
            double_u->compile();
            robot.root->anchors[i]->instance = double_u;

            ComponentInstance *side = backend.getComponent("side_to_side")->instanciate();
            side->compile();
            double_u->anchors[0]->instance = side;
            double_u->anchors[0]->alpha = -DEG2RAD(30);

            ComponentInstance *leg = backend.getComponent("arm_leg")->instanciate();
            leg->compile();
            side->anchors[0]->instance = leg;
            side->anchors[0]->alpha = DEG2RAD(85);
        }
        
        // Getting 3D model
        robot.computeModel();
        Model model = robot.toModel();
        saveModelToFileAscii("/tmp/demo.stl", &model);

    } catch (string error) {
        cerr << "[ERROR] " << error << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
