#include <iostream>
#include "ExperimentAnalyze.h"

static void dump(Controller::Leg &leg, unsigned int n=0)
{
    float min, max;
    unsigned int N = 0;
    for (auto &entry : leg.tip.chain) {
        if (entry.type == CHAIN_ROTATION) {
            if (n == N) {
                min = entry.min;
                max = entry.max;
                break;
            }
            N++;
        }
    }
    for (float a=min; a<max; a+=0.25) {
        leg.alphas[n] = a;
        if (n+1 < leg.alphas.size()) {
            dump(leg, n+1);
        } else {
            auto pos = leg.tip.position(leg.alphas);
            std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
        }
    }
}
        
void ExperimentAnalyzeDump::control(Simulation *simulation)
{
    ExperimentController::control(simulation);

#if 0
    // Dumping leg positions
    auto &leg = controller->legs[0];
    auto pos = leg.tip.position(leg.alphas);
    std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
#endif

#if 1
    if (simulation->t > 0.5) {
        // Dumping reachable space
        for (auto &leg : controller->legs) {
            dump(leg);
            exit(0);
        }
        exit(0);
    }
#endif
}

ExperimentAnalyzePath::ExperimentAnalyzePath()
    : cop(0, 0, 0)
{
    std::cout << "# Path analysis" << std::endl;
    std::cout << "# Fields are:" << std::endl;
    std::cout << "# 1: simulation time" << std::endl;
    std::cout << "# 2, 3, 4: first path tip (robot frame)" << std::endl;
    std::cout << "# 5, 6, 7: com position (world frame)" << std::endl;
    std::cout << "# 8, 9, 10: cop position (world frame)" << std::endl;
    std::cout << "# 11, 12, 13: trunk position (world frame)" << std::endl;
}

void ExperimentAnalyzePath::control(Simulation *simulation)
{
    ExperimentController::control(simulation);

    // Dumping leg positions
    auto &leg = controller->legs[0];
    auto tip = leg.tip.position(leg.alphas);

    auto st = simulation->robot.root->getState();
    auto pos = simulation->robot.getBulletCOM();
    auto tmp = cop.multiply(0.97);
    cop = simulation->robot.getCollisionsCOP().multiply(0.03).add(tmp);
    std::cout << 
        // 1
        simulation->t << " " <<
        // 2 3 4
        tip.x << " " << tip.y << " " << tip.z << " " <<
        // 5 6 7
        pos.x() << " " << pos.y() << " " << pos.z() << " " <<
        // 8 9 10
        cop.x() << " " << cop.y() << " " << cop.z() << " " <<
        // 11 12 13
        st.x()/1000.0 << " " << st.y()/1000.0 << " " << st.z()/1000.0 << std::endl
        ;
}

void ExperimentAnalyzeInitial::control(Simulation *simulation)
{
    ExperimentController::control(simulation);

    if (simulation->t > 0.5) {
        // Dumping reachable space
        for (auto &leg : controller->legs) {
            auto pos = leg.tip.position(leg.alphas);
            std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
            exit(0);
        }
        exit(0);
    }
}
