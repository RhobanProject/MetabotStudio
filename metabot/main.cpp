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
#include "Backend.h"
#include "AnchorPoint.h"
#include "Component.h"
#include "Robot.h"
#include "Voxels.h"
#include "util.h"

using namespace std;
using namespace Metabot;

Backend *backend = NULL;
Robot *robot = NULL;
string robotFile = "";

void usage()
{
    cerr << "Metabot v1.0 - Rhoban System" << endl;
    cerr << "Usage: metabot [option] [file.robot]" << endl;
    cerr << "" << endl;
    cerr << "Cache operations" << endl;
    cerr << "   -c: Clears the cache" << endl;
    cerr << "   -w: Warmup/generates the cache" << endl;
    cerr << "" << endl;
    cerr << "Robot operations (needs a .robot)" << endl;
    cerr << "   -s [output.stl]: Export the given robot to stl" << endl;
    cerr << "   -p: shows the list of parts to print (stl and quantity)" << endl;
    cerr << "   -b: shows the BOM" << endl;
    cerr << "   -u: saves to URDF" << endl;
    cerr << "Dev:" << endl;
    cerr << "   -v: voxelize" << endl;
    exit(EXIT_FAILURE);
}

void needRobot()
{
    if (robotFile != "") {
        try {
            robot = new Robot(backend);
            robot->loadFromFile(robotFile);
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

    while ((index = getopt(argc, argv, "pbcws:vo:u")) != -1) {
        switch (index) {
            case 'c':
                mode = "cacheClear";
                break;
            case 'w':
                mode = "cacheWarmup";
                break;
            case 's':
                mode = "stlExport";
            case 'o':
                output = string(optarg);
                break;
            case 'b':
                mode = "bom";
                break;
            case 'p':
                mode = "parts";
                break;
            case 'v':
                mode = "voxelize";
                break;
            case 'u':
                mode = "urdf";
                break;
        }
    }

    if (optind != argc) {
        robotFile = string(argv[optind]);
    }

    try {
        // Loading the backend
        backend = new Backend("xl-320");
        backend->load();

        // Cache handling
        if (mode == "cacheClear") {
            cout << "Clearing the cache" << endl;
            int n = backend->clearCache();
            cout << "Done, removed " << n << " files." << endl;
        } else if (mode == "cacheWarmup") {
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
        } else if (mode == "urdf") {
            needRobot();
            robot->writeURDF(output);
        } else if (mode == "voxelize") {
            Model m = loadModelFromFile(robotFile.c_str());
            //m.gnuplot();
            Voxels::voxelize(m);
            
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

    if (backend != NULL) {
        delete backend;
    }

    return EXIT_SUCCESS;
}
