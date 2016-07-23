#include <iostream>
#include "ExperimentAnalyze.h"

static void dump(Controller::Leg &leg, int n=0)
{
    auto &chain = leg.tip.chain;
    float min, max;
    int N = 0;
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
        
void ExperimentAnalyze::control(Simulation *simulation)
{
    ExperimentController::control(simulation);

    for (auto &leg : controller->legs) {
        auto pos = leg.tip.position(leg.alphas);
        std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;

        dump(leg);
        exit(0);
    }

    exit(0);
}
