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
        if (item.type == CHAIN_ROTATION) {
            alphas.push_back(0.0);
        }
    }

    // Leg vector
    auto pos = tip.position(alphas);
    pos.z = 0;
    xVec = pos.x;
    yVec = pos.y;

    // Theta is between 0 and 2pi
    theta = atan2(yVec, xVec);
    if (theta < 0) theta += 2*M_PI;
}
    
float Controller::Leg::error(std::vector<double> &candidate, float x, float y, float z)
{
    auto pos = tip.position(candidate);
    return pow(pos.x-x, 2) + pow(pos.y-y, 2) + pow(pos.z-z, 2);
}

void Controller::Leg::gotoXYZ_cmaes(float x, float y, float z)
{
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
    for (k=0; k<dim; k++) {
        alphas[k] = X[k];
    }
}

void Controller::Leg::gotoXYZ_rand(float x, float y, float z)
{
    float err, newErr = error(alphas, x, y, z);

    do {
        err = newErr;
        for (int k=0; k<30; k++) {
            auto tmp = alphas;
            for (int n=0; n<tmp.size(); n++) {
                if (k < 3) {
                    tmp[n] += 0.1-0.2*rand()/(float)RAND_MAX;
                } else {
                    tmp[n] += 0.01-0.02*rand()/(float)RAND_MAX;
                }
            }
            auto test = error(tmp, x, y, z);
            if (test < err) {
                newErr = test;
                alphas = tmp;
            }
        }
    } while (newErr < err);
}

void Controller::Leg::gotoXYZ(float x, float y, float z)
{
    gotoXYZ_rand(x,y,z);
}

Controller::Controller(Metabot::Robot *robot)
{
    // Phases
    int tips = robot->tips();
    phases = new float[tips];

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
    dx = 0;
    dy = 0;
    turn = 0;

    // Locus
    support=0.5;
    lX = 0;
    lH = 30;
    lS = 1;

    ut = 1.0;
}

void Controller::setupFunctions()
{
    step = Leph::CubicSpline();
    rise = Leph::CubicSpline();
    
    step.addPoint(0, 0.5, -1/support);
    step.addPoint(support, -0.5, -1/support);
    step.addPoint(support+(1-support)/2, lX, lS/support);
    step.addPoint(1, 0.5, -1/support);

    rise.addPoint(0, 0, 0);
    rise.addPoint(support, 0, 0);
    rise.addPoint(support+(1-support)/2, lH, 0);
    rise.addPoint(1, 0, 0);

    /*
    for (float t=0; t<1; t+=0.01) {
        printf("%g %g %g\n", t, step.pos(t), rise.pos(t));
    }
    exit(0);
    */
}

void Controller::compute(float t_)
{
    float t = freq*t_;

    int k = 0;
    auto &leg = legs[0];
    for (auto &leg : legs) {
        float phase = phases[k++] + t;

        // Following the spline
        float tx = leg.xVec*x;
        float ty = leg.yVec*y;

        // Applying rotation
        float tx_ = tx;
        float ty_ = ty;
        float theta = step.posMod(phase)*turn;
        tx = cos(theta)*tx_ - sin(theta)*ty_;
        ty = sin(theta)*tx_ + cos(theta)*ty_;

        // Applying trajectory
        tx += step.posMod(phase)*dx;
        ty += step.posMod(phase)*dy;

        float tz = rise.posMod(phase) - z;
        
        leg.gotoXYZ(tx, ty, tz);
    }
}
        
double Controller::update(float dt, float t, Metabot::Robot &robot)
{
    // Refresh controller at 50hz
    ut += dt;
    if (ut > 0.02) {
        compute(t);
        ut = 0;
    }

    // Sets the target on each component
    double cost = 0;
    for (auto &leg : legs) {
        int k = 0;
        for (auto &item : leg.tip.chain) {
            if (item.type == CHAIN_ROTATION) {
                auto alpha = -leg.alphas[k++];
                cost += fabs(robot.getComponentById(item.jointId)->setTarget(alpha, dt));
            }
        }
    }

    // The total cost is the sum of the cost of each joint
    return cost;
}
