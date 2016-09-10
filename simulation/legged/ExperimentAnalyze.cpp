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

void ExperimentAnalyzeDraw::control(Simulation *simulation)
{
    for (auto &leg : controller->legs) {
        leg.dummy = true;
    }
    double bestScore = 1e9;
    double bestp2, bestp3, bestp4;
            
    {
        std::ofstream svg("gait.svg");
        boost::geometry::svg_mapper<boostPoint> mapper(svg, 400, 400, "");

        int yDivide = 0;
        double xOffset = 0;
        double yOffset = 0;
        double score = 0;
        double over = 0;
        for (double t=0.0; t<1.0; t+=0.03) {
            xOffset += 500;
            yDivide++;
            if (yDivide > 5) {
                yDivide = 0;
                yOffset += 400;
            }
            boostPolygon poly;
            controller->compute(t);
           
            bool isFirst = true;
            boostPoint first;
            for (int k=0; k<4; k++) {
                auto &leg = controller->legs[k];
                if (leg.zTarget < (-controller->z+1)) {
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
            mapper.map(poly, "fill:#70c4ff");
            mapper.map(x, "fill:#8c1593", 15);
        
            for (int k=0; k<4; k++) {
                auto &leg = controller->legs[k];
                boostPoint point(xOffset+leg.xTarget, leg.yTarget);
                mapper.add(point);
            
                if (leg.zTarget < (-controller->z+1)) {
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
        }
    }
    exit(0);
}
 
#define MODE_BEST

void ExperimentAnalyzeStable::control(Simulation *simulation)
{
    typedef boost::tuple<float, float> boostPoint;
    typedef bg::model::polygon<boostPoint> boostPolygon;

    for (auto &leg : controller->legs) {
        leg.dummy = true;
    }

#ifdef MODE_BEST
    double bestScore = 1e9;
    double bestp2, bestp3, bestp4;
#endif
                        
    for (double p2=0; p2<1; p2+=0.03) {
        controller->phases[1] = p2;
        for (double p3=0; p3<1; p3+=0.03) {
            controller->phases[2] = p3;
            for (double p4=0; p4<1; p4+=0.03) {
                controller->phases[3] = p4;
    
                double score = 0;
                double over = 0;
                for (double t=0.0; t<1.0; t+=0.01) {
                    boostPolygon poly;
                    controller->compute(t);
                   
                    bool isFirst = true;
                    boostPoint first;
                    for (int k=0; k<4; k++) {
                        auto &leg = controller->legs[k];
                        if (leg.zTarget < (-controller->z+1)) {
                            boostPoint point(leg.xTarget, leg.yTarget);
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

                    if (bg::num_points(poly) > 0) {
                        if (!bg::within(boostPoint(0, 0), poly)) {
                            score += bg::distance(boostPoint(0, 0), poly);
                        } else {
                            score -= bg::distance(boostPoint(0, 0), poly);
                        }
                    } else {
                        score += 1e6;
                    }

#ifdef MODE_BEST
                    if (score > bestScore) break;
#endif
                }
                exit(0);

#ifdef MODE_BEST
                if (score < bestScore) {
                    bestScore = score;
                    bestp2 = p2;
                    bestp3 = p3;
                    bestp4 = p4;
                }
#else
                if (score < 500) {
                    double v3 = p3+0.5;
                    if (v3 > 1) v3 -= 1;
                    printf("%g %g %g %g\n", score, p2, v3, p4);
                }
#endif
            }
        }
    }
 
#ifdef MODE_BEST
    double v3 = bestp3+0.5;
    if (v3 > 1) v3 -= 1;
    printf("%g %g %g %g\n", bestScore, bestp2, v3, bestp4);
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
