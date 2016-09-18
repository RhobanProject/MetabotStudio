#include <fstream>
#include <iostream>
#include "ExperimentAnalyze.h"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/adapted/boost_tuple.hpp>
#include <boost/geometry/geometries/register/point.hpp>

namespace bg = boost::geometry;
BOOST_GEOMETRY_REGISTER_BOOST_TUPLE_CS(cs::cartesian)

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
    // Dumping reachable space
    for (auto &leg : controller->legs) {
        dump(leg);
        exit(0);
    }
    exit(0);
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

double ExperimentAnalyzePath::score(Simulation *simulation)
{
    exit(0);
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
        // tip.x << " " << tip.y << " " << tip.z << " " <<
        leg.xTarget << " " << leg.yTarget << " " << leg.zTarget << " " << 
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

typedef boost::tuple<float, float> boostPoint;
typedef bg::model::polygon<boostPoint> boostPolygon;
typedef bg::model::segment<boostPoint> boostSegment;

void ExperimentAnalyzeDraw::control(Simulation *simulation)
{
    for (auto &leg : controller->legs) {
        leg.dummy = true;
    }
    controller->freq = 1;

    {
        std::ofstream svg("gait.svg");
        boost::geometry::svg_mapper<boostPoint> mapper(svg, 300, 300, "");

        int yDivide = 0;
        double xOffset = 0;
        double yOffset = 0;
        double score = 0;
        double over = 0;
        for (double t=0.0; t<1.0; t+=0.075) {
            boostPolygon poly;
            controller->compute(t);
            std::cout << xOffset << " / " << yOffset << std::endl;

            bool isFirst = true;
            boostPoint first;
            for (int k=0; k<controller->legs.size(); k++) {
                auto &leg = controller->legs[k];
                if (leg.zTarget < (-controller->z+1e-3)) {
                    boostPoint point(xOffset+leg.xTarget, yOffset+leg.yTarget);
                    if (isFirst) {
                        first = point;
                        isFirst = false;
                    }
                    bg::append(poly, point);
                    //printf("(%g,%g)\n", leg.xTarget, leg.yTarget);
                }
            }
            if (!isFirst) {
                bg::append(poly, first);
            }

            boostPoint x(xOffset, yOffset);
            mapper.add(poly);
            mapper.add(x);
            mapper.map(poly, "fill:#70c4ff; stroke:#70c4ff; stroke-width:10");
            mapper.map(x, "fill:#8c1593", 15);

            for (int k=0; k<4; k++) {
                auto &leg = controller->legs[k];
                boostPoint point(xOffset+leg.xTarget, leg.yTarget+yOffset);
                mapper.add(point);

                if (leg.zTarget < (-controller->z+1e-3)) {
                    mapper.map(point, "fill:black;stroke:black", 10);
                } else {
                    mapper.map(point, "fill:yellow;stroke:black", 10);
                }
            }

            if (bg::num_points(poly) > 0) {
                if (!bg::within(boostPoint(0, 0), poly)) {
                    score += bg::distance(boostPoint(0, 0), poly);
                } else {
                    score -= bg::distance(boostPoint(0, 0), poly);
                }
            } else {
                score += 1e6;
            }

            xOffset += 650;
            yDivide++;
            if (yDivide > 4) {
                yDivide = 0;
                xOffset = 0;
                yOffset -= 550;
            }
        }
    }
    exit(0);
}

void makePossibilities(
        std::vector<std::vector<float>> &possibilities,
        int size, double epsilon = 0.01,
        std::vector<float> prefix = std::vector<float>()
        )
{
    for (double t=0; t<1.0; t+=epsilon) {
        auto tmp = prefix;
        tmp.push_back(t);

        if (tmp.size() < size) {
            makePossibilities(possibilities, size, epsilon, tmp);
        } else {
            possibilities.push_back(tmp);
        }
    }
}

#define MODE_BEST

void ExperimentAnalyzeStable::control(Simulation *simulation)
{
    for (auto &leg : controller->legs) {
        leg.dummy = true;
    }
    controller->freq = 1;

#ifdef MODE_BEST
    double bestScore = 1e9;
    std::vector<float> best;
#endif

    std::vector<std::vector<float>> possibilities;
    makePossibilities(possibilities, controller->legs.size()-1);

    long int N = 0;
    for (auto &phases : possibilities) {
#ifdef MODE_BEST
        N++;
        double f = 100*N/(double)possibilities.size();
        //printf("%02.2f%\r", f);
        for (int k=0; k<phases.size(); k++) {
            controller->phases[k+1] = phases[k];
        }
#endif

        /*
        for (auto p : phases) {
            printf("%g ", p);
        }
        printf("\n");
        */

        double score = 0;
        double sscore = 0;
        double over = 0;
        for (double t=0.0; t<1.0; t+=0.03) {
            std::vector<boostSegment> segments;
            boostPolygon poly;
            controller->compute(t);

            bool isFirst = true;
            boostPoint first;
            boostPoint last;
            for (int k=0; k<controller->legs.size(); k++) {
                auto &leg = controller->legs[k];
                if (leg.zTarget < (-controller->z+1e-3)) {
                    boostPoint point(leg.xTarget, leg.yTarget);
                    if (isFirst) {
                        first = point;
                        isFirst = false;
                    } else {
                        segments.push_back(boostSegment(last, point));
                    }
                    last = point;
                    bg::append(poly, point);
                    //printf("(%g,%g)\n", leg.xTarget, leg.yTarget);
                }
            }
            if (!isFirst) {
                bg::append(poly, first);
            }

            if (bg::num_points(poly) > 0) {
                if (!bg::within(boostPoint(0, 0), poly)) {
                    score += bg::distance(boostPoint(0, 0), poly);
                } else {
                    boostPoint c(0, 0);
                    bool isFirst = true;
                    double nearest;
                    for (auto s : segments) {
                        double dist = bg::distance(c, s);
                        if (isFirst) {
                            isFirst = false;
                            nearest = dist;
                        } else {
                            if (dist < nearest) nearest = dist;
                        }
                    }

                    sscore += nearest;
                }
            } else {
                score += 1e6;
            }

#ifdef MODE_BEST
        /*
        double vscore = score;
        if (vscore < 0.1) {
            vscore = -sscore;
        }
        if (vscore > bestScore) break;
        */
#endif
        }

        if (score < 0.1) {
            score = -sscore;
        }

        // printf("=> %f\n", score);

#ifdef MODE_BEST
        if (score < bestScore) {
            bestScore = score;
            best = phases;
        }
#else

        //if (score < 500) {
            if (controller->legs.size() == 4) {
                phases[1] += 0.5;
                if (phases[1] > 1) phases[1] -= 1;
            }
            printf("%f ", score);
            for (auto p : phases) printf("%f ", p);
            printf("\n");
        //}
        exit(0);
#endif
    }

#ifdef MODE_BEST
    if (controller->legs.size() == 4) {
        best[1] += 0.5;
        if (best[1] > 1) best[1] -= 1;
    }
    printf("%f ", bestScore);
    for (auto p : best) printf("%f ", p);
    printf("\n");
#endif

    exit(0);
}

void ExperimentAnalyzeEffectiveSupport::init(Simulation *simulation, Parameters &parameters)
{
    ExperimentController::init(simulation, parameters);

    totalFrames = 0;

    simulation->robot.foreachComponent([this](Component *component, TransformMatrix m) {
            if (component->tips.size()) {
            legs.push_back(component);
            supportFrames.push_back(0);
            }
            });
}

void ExperimentAnalyzeEffectiveSupport::control(Simulation *simulation)
{
    ExperimentController::control(simulation);

    double total = 0;
    for (auto leg : legs) {
        double collisions = simulation->robot.world.getGroundCollisions(leg->body);
        total += collisions;
    }

    int k = 0;
    for (auto leg : legs) {
        double collisions = simulation->robot.world.getGroundCollisions(leg->body);

        if (collisions/total > 0.15) {
            supportFrames[k]++;
        }
        k++;
    }

    totalFrames++;
}


double ExperimentAnalyzeEffectiveSupport::score(Simulation *simulation)
{
    double f = 0;
    for (auto s : supportFrames) {
        f += s/(double)totalFrames;
    }
    f /= legs.size();

    printf("%g\n", f);
    exit(0);
}
