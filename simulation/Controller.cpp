#include <stdio.h>
#include <math.h>
#include <cmaes.h>
#include <Component.h>
#include "kinematic.h"
#include "Controller.h"

using namespace libcmaes;
            
Controller::Leg::Leg(Metabot::Kinematic::Tip tip_)
    : tip(tip_)
{
    for (auto &item : tip.chain) {
        item.alpha = 0.0;
    }
    auto pos = tip.position();
    pos.z = 0;
    auto norm = pos.norm();

    xVec = pos.x/norm;
    yVec = pos.y/norm;
    theta = atan2(yVec, xVec);
    if (theta < 0) theta += 2*M_PI;
}
    
float Controller::Leg::error(std::vector<float> deltas, float x, float y, float z)
{
    auto tmp = tip;
    int k = 0;
    for (auto &item : tmp.chain) {
        if (item.type == CHAIN_ROTATION) {
            item.alpha += deltas[k++];
            if (item.alpha < item.min) item.alpha = item.min+0.05;
            if (item.alpha > item.max) item.alpha = item.max-0.05;
        }
    }
    auto pos = tmp.position();
    // printf("Test: (%g %g) %g/%g %g/%g %g/%g\n", xVec, yVec, pos.x, x, pos.y, y, pos.z, z);

    return pow(pos.x-x, 2) + pow(pos.y-y, 2) + pow(pos.z-z, 2);
}

void Controller::Leg::gotoXYZ_cmaes(float x, float y, float z)
{
    auto alphas = tip.alphas();
    int dim = alphas.size();
    CMAParameters<> cmaparams(alphas, -1);
    // cmaparams.set_algo(BIPOP_CMAES);
    // cmaparams.set_quiet(false);
    Point3 target(x, y, z);

    FitFunc kin = [this, &target](const double *x, const int N) {
        std::vector<double> alphas;
        for (int k=0; k<N; k++) {
            if (x[k] < -M_PI) return 1e6;
            if (x[k] > M_PI) return 1e6;
            alphas.push_back(x[k]);
        }
        auto positions = this->tip.positions(alphas);

        auto end = positions[positions.size()-1];
        auto score = target.distance(end);

        double p = 0;
        for (int k=0; k<positions.size()-2; k++) {
            auto &pos = positions[k];
            pos.z += 100;
            if (pos.z > 0) {
                p += pos.z*pos.z;
            }
            auto Y = this->yVec*pos.y;
            if (Y > 0) p += Y*Y;
            auto X = this->xVec*pos.x;
            if (X > 0) p += X*X;
        }
        score += 1e7/(1.0+p);

        return score;
    };

    CMASolutions cmasols = cmaes<>(kin, cmaparams);
    auto best = cmasols.get_best_seen_candidate();
    auto X = best.get_x_ptr();
    int k = 0;
    //std::cout << "Candidate: ";
    for (auto &item : tip.chain) {
        if (item.type == CHAIN_ROTATION) {
            //std::cout << X[k] << " ";
            item.alpha = X[k++];
        }
    }
    //std::cout << std::endl;
    //std::cout << "Best candidate value: " << best.get_fvalue() << std::endl;
    auto end = tip.position();
    //printf("%g/%g, %g/%g, %g/%g\n",
    //        end.x, target.x, end.y, target.y, end.z, target.z);
}

void Controller::Leg::gotoXYZ_rand(float x, float y, float z)
{
    std::vector<float> deltas;
    for (auto item : tip.chain) {
        if (item.type == CHAIN_ROTATION) {
            deltas.push_back(0.0);
        }
    }
    float err, newErr = error(deltas, x, y, z);

    do {
        err = newErr;
        for (int k=0; k<30; k++) {
            std::vector<float> tmpD = deltas;
            for (int n=0; n<tmpD.size(); n++) {
                if (k < 3) {
                    tmpD[n] += 0.1-0.2*rand()/(float)RAND_MAX;
                } else {
                    tmpD[n] += 0.01-0.02*rand()/(float)RAND_MAX;
                }
            }
            auto test = error(tmpD, x, y, z);
            if (test < err) {
                newErr = test;
                deltas = tmpD;
            }
        }
    } while (newErr < err);

    int k = 0;
    for (auto &item : tip.chain) {
        if (item.type == CHAIN_ROTATION) {
            item.alpha += deltas[k++];
            if (item.alpha < item.min) item.alpha = item.min+0.05;
            if (item.alpha > item.max) item.alpha = item.max-0.05;
        }
    }
}

void Controller::Leg::gotoXYZ(float x, float y, float z)
{
    gotoXYZ_rand(x,y,z);
}

Controller::Controller(Metabot::Robot *robot, float l1, float l2, float l3)
    : l1(l1), l2(l2), l3(l3)
{
    // Getting legs
    auto kinematic = robot->computeKinematic();

    for (auto tip : kinematic.tips) {
        Leg leg(tip);
        legs.push_back(leg);
    }

    // Sorting legs using theta
    std::sort(legs.begin(), legs.end(), [](const Leg &a, const Leg &b) {
        return (a.theta < b.theta);
    });

    // Initialisation
    gait = GAIT_TROT;
    dx = 0;
    dy = 0;
    turn = 0;
    ut = 1.0;
    setupFunctions();
}

void Controller::setupFunctions()
{
    rise.clear();
    step.clear();

    if (gait == GAIT_WALK) {
        phases[0] = 0.0;
        phases[1] = 0.5;
        phases[2] = 0.75;
        phases[3] = 0.25;
        
        // Rising the legs
        rise.addPoint(0.0, 0.0);
        rise.addPoint(0.1, 1.0);
        rise.addPoint(0.3, 1.0);
        rise.addPoint(0.35, 0.0);
        rise.addPoint(1.0, 0.0);

        // Taking the leg forward
        step.addPoint(0.0, -0.5);
        step.addPoint(0.12, -0.5);
        step.addPoint(0.3, 0.5);
        step.addPoint(0.35, 0.5);
        step.addPoint(1.0, -0.5);
    }

    if (gait == GAIT_TROT) {
        phases[0] = 0.0;
        phases[1] = 0.5;
        phases[2] = 0.0;
        phases[3] = 0.5;

        // Rising the legs
        rise.addPoint(0.0, 1.0);
        rise.addPoint(0.3, 1.0);
        rise.addPoint(0.4, 0.0);
        rise.addPoint(0.9, 0.0);
        rise.addPoint(1.0, 1.0);

        // Taking the leg forward
        step.addPoint(0.0, -0.5);
        step.addPoint(0.1, -0.5);
        step.addPoint(0.3, 0.5);
        step.addPoint(0.5, 0.5);
        step.addPoint(0.85, -0.5);
        step.addPoint(1.0, -0.5);

        /*
         // Rising the legs
         rise.addPoint(0.0, 0.0);
         rise.addPoint(0.1, 1.0);
         rise.addPoint(0.4, 1.0);
         rise.addPoint(0.5, 0.0);
         rise.addPoint(1.0, 0.0);
 
         // Taking the leg forward
         step.addPoint(0.0, -0.5);
         step.addPoint(0.1, -0.5);
         step.addPoint(0.5, 0.5);
         step.addPoint(1.0, -0.5);
         */
    }
}

void Controller::compute(float t_)
{
    float t = freq*t_;
    float turnRad = DEG2RAD(turn);

    int k = 0;
    auto &leg = legs[0];
    for (auto &leg : legs) {
        k++;
        float phase = t + 0.5*(k%2);

        // Following the spline
        float tx = leg.xVec*x + step.getMod(phase)*dx;
        float ty = leg.yVec*y + step.getMod(phase)*dy;
        float tz = rise.getMod(phase)*alt - z;
        
        leg.gotoXYZ(tx, ty, tz);
    }
}
        
double Controller::update(float dt, float t, Metabot::Robot &robot)
{
    double cost = 0;
    ut += dt;

    if (ut > 0.02) {
        compute(t);
        ut = 0;
    }

    for (auto &leg : legs) {
        for (auto &item : leg.tip.chain) {
            if (item.type == CHAIN_ROTATION) {
                cost += fabs(robot.getComponentById(item.jointId)->setTarget(-item.alpha, dt));
            }
        }
    }

    return cost;
}
