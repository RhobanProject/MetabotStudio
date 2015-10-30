#include <string>
#include <iostream>
#include <3d/stl.h>
#include <3d/Face.h>
#include <3d/Line.h>
#include <3d/Point3.h>
#include <3d/Octree.h>
#include <3d/Volume.h>
#include <getopt.h>
#include "util.h"
#include "AnchorPoint.h"
#include "Component.h"
#include "Robot.h"
#include "Voxels.h"
#include "TransformMatrix.h"
#include "Backend.h"
#include "util.h"

using namespace std;
using namespace Metabot;

Robot *robot = NULL;
string robotFile = "";

void usage()
{
    cerr << "Metabot v1.0 - Rhoban System" << endl;
    cerr << "Usage: metabot [option] [file.robot]" << endl;
    cerr << "" << endl;
    cerr << "Backend operations" << endl;
    cerr << "   -c [backend]: Clears the cache" << endl;
    cerr << "   -w [backend]: Warmup/generates the cache" << endl;
    cerr << "" << endl;
    cerr << "Robot operations (needs a .robot)" << endl;
    cerr << "   -s [output.stl]: Export the given robot to stl" << endl;
    cerr << "   -p [output directory]: exports the parts to print (stl and quantity)" << endl;
    cerr << "   -S [output directory]: saves to SDF" << endl;
    cerr << "   -j [output directory]: save to JS (three loading operation)" << endl;
    cerr << "   -b: shows the BOM" << endl;
    cerr << "   -d: compute the dynamics" << endl;
    cerr << "   -k: outputs the direct kinematics" << endl;
    exit(EXIT_FAILURE);
}

Values defines;

void needRobot()
{
    if (robotFile != "") {
        try {
            robot = new Robot();
            robot->loadFromFile(robotFile, defines);
        } catch (string err) {
            cerr << "Error: unable to open " << robotFile << " (" << err << ")" << endl;
            exit(EXIT_FAILURE);
        } catch (...) {
            cerr << "Error: unable to open " << robotFile << endl;
            exit(EXIT_FAILURE);
        }
    } else {
        cerr << "Error: no robot given" << endl;
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    int index;
    string mode = "";
    string output = "";
    string backendName = "";

    while ((index = getopt(argc, argv, "p:bc:w:s:vS:dj:kD:")) != -1) {
        switch (index) {
            case 'D': {
                    auto parts = split(string(optarg), '=', 2);
                    if (parts.size() == 2) {
                        defines.set(parts[0], parts[1]);
                    }
                }
                break;
            case 'c':
                backendName = string(optarg);
                mode = "cacheClear";
                break;
            case 'w':
                backendName = string(optarg);
                mode = "cacheWarmup";
                break;
            case 's':
                output = string(optarg);
                mode = "stlExport";
                break;
            case 'b':
                mode = "bom";
                break;
            case 'p':
                output = string(optarg);
                mode = "parts";
                break;
            case 'v':
                mode = "voxelize";
                break;
            case 'S':
                output = string(optarg);
                mode = "sdf";
                break;
            case 'd':
                mode = "dynamics";
                break;
            case 'j':
                output = string(optarg);
                mode = "js";
                break;
            case 'k':
                mode = "kinematic";
                break;
        }
    }

    if (optind != argc) {
        robotFile = string(argv[optind]);
    }

    /*
    auto a = TransformMatrix::identity();
    a = a.multiply(TransformMatrix::rotationZ((30)*M_PI/180));
    a = a.multiply(TransformMatrix::rotationY((20)*M_PI/180));
    a = a.multiply(TransformMatrix::rotationX((18)*M_PI/180));
    auto rpy = eulerAngles(a.values);
    std::cout << "A" << std::endl;
    std::cout << a.toString() << std::endl;

    std::cout << "X: " << (rpy[0]*180/M_PI) << std::endl;
    std::cout << "Y: " << (rpy[1]*180/M_PI) << std::endl;
    std::cout << "Z: " << (rpy[2]*180/M_PI) << std::endl;
    return 0;
    */

    try {
        // Cache handling
        if (mode == "cacheClear") {
            auto backend = Backend::get(backendName);
            cout << "Clearing the cache" << endl;
            int n = backend->clearCache();
            cout << "Done, removed " << n << " files." << endl;
        } else if (mode == "cacheWarmup") {
            auto backend = Backend::get(backendName);
            cout << "Generating the cache..." << endl;
            backend->buildCache();
            cout << "Cache generated (" << backend->cacheFiles() << " files)." << endl;
        // Robot handling
        } else if (mode == "stlExport") {
            needRobot();
            cout << "Saving robot to " << output << endl;
            auto model = robot->toModel();
            saveModelToFileBinary(output.c_str(), &model);
        } else if (mode == "bom") {
            needRobot();
            cout << "Bill of materials" << endl;
            cout << robot->getBOM().toString();
        } else if (mode == "parts") {
            needRobot();
            auto groups = robot->writeSTLs(output);
            cout << "Parts" << endl;
            std::cout << groups.toString() << std::endl;
        } else if (mode == "dynamics") {
            needRobot();
            robot->printDynamics();
        } else if (mode == "js") {
            needRobot();
            robot->writeJS(output);
        } else if (mode == "sdf") {
            needRobot();
            robot->writeSDF(output);
        } else if (mode == "voxelize") {
            Model m = loadModelFromFile(robotFile.c_str());
            //m.gnuplot();
            Voxels::voxelize(m);

        /*
        std::cout << "Box volume: " << volume << std::endl;
        std::cout << "Inside: " << inside << ", Outside: " << outside << std::endl;
        std::cout << "Total: " << (100*ratio) << "%" << std::endl;
        std::cout << "Monte carlo volume: " << (ratio*volume) << std::endl;
        */
            
            //Line l(Point3(15.5,26.5,0.5),Point3(0.817064,-0.229817,1.82707));
            //Face f(Point3(17.2,7.73939,8.73626),Point3(17.2,7.48578,8.44599),Point3(17.2,45,-9.99201e-15));
            //f.gnuplot();

            /*
            Model m;
            Volume v;
            auto f1 = Face(Point3(3,3,3), Point3(13,-4,3), Point3(6,7,6));
            auto f2 = Face(Point3(2.6,0.3,8.4), Point3(0.2,0.6,0.3), Point3(1.2,2.4,-0.5));
            v.faces.push_back(f1);
            v.faces.push_back(f2);
            m.volumes.push_back(v);
            */

            //Line l(Point3(14.5,-31.5,0.5),Point3(0.817064,-0.229817,1.82707));
            /*
            Face f(Point3(17.2,-15,20),Point3(17.2,-45,11),Point3(17.2,-15,3.33067e-15));
            if (c.intersects(f)) {
                std::cout << "intersects!!!!!!!" << std::endl;
            }
            */
            //c.gnuplot();

            //m.gnuplot();
            //Octree octree;
            //octree.load(m);
            //octree.facesFor(l);
            //Cube c(10, 20, 22, 40, 3, 7);
            //c.gnuplot();

            /*
            if (c.intersects(f)) {
                std::cout << "INTERSECTS!" << std::endl;
            }
            */

            //OctreeFloor floor(Cube(0, 1, 0, 1, 0, 1), 3);
            //Face f(Point3(0.7,0.22,0.3), Point3(0.72,0.25,0.34), Point3(0.71,0.23,0.35));
            //floor.add(f);        
            //Line l(Point3(3,4,2), Point3(0.4, -0.7, 0.2));
            //l.gnuplot();
            //auto faces = octree.facesFor(l);
            //for (auto face : faces) {
                //face->gnuplot();
            //}
            //Cube c(3.9, 4.1, 5.5, 5.7, 2.46, 2.8);
            // c.gnuplot();
            //c.intersects(l);
        } else if (mode == "kinematic") {
            needRobot();
            auto kinematic = robot->computeKinematic();
            kinematic.display();

            /*
            for (auto tip : kinematic.tips) {
                auto point = tip.chain.position();
                std::cout << point.x << ", " << point.y << ", " << point.z << std::endl;
                for (auto item : tip.chain.items) {
                    if (item.type == CHAIN_ROTATION) {
                        std::cout << "Rotation with alpha " << item.alpha << std::endl;
                    } else {
                        std::cout << "Matrix:" << std::endl;
                        std::cout << item.matrix.toString() << std::endl;
                    }
                }
            }
            */
        } else {
            usage();
        }

    } catch (string error) {
        cerr << "[ERROR] " << error << endl;
        return EXIT_FAILURE;
    }

    if (robot != NULL) {
        delete robot;
    }

    return EXIT_SUCCESS;
}
