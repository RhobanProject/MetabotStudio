#include <iostream>
#include <vector>
#include <3d/Octree.h>
#include "Voxels.h"
#include "util.h"

//#define METHOD_RANDOM

#define DX  0.8170638283522394
#define DY -0.2298170729638312
#define DZ  1.8270672983620736

namespace Metabot
{
    Dynamics Voxels::voxelize(Model m, double density, double mass)
    {
        float resolution = 1.0;
#ifdef METHOD_RANDOM
        srand (time(NULL));
#endif

        // Zero point
        auto zero = m.min();
        auto max = m.max();
        Point3 com(0, 0, 0);

        // Dimensions
        int width = (max.x+1-zero.x)/resolution;
        int height = (max.y+1-zero.y)/resolution;
        int depth = (max.z+1-zero.z)/resolution;
        Dynamics dynamics;

        int cubes = 0;

        Octree octree;
        octree.load(m);

#ifdef METHOD_RANDOM
        for (int i=0; i<width*height*depth; i++)
#else
        for (int z=0; z<depth; z++) 
        for (int y=0; y<height; y++)
        for (int x=0; x<width; x++)
#endif
        {
            int left = 0;
            int right = 0;
            float X, Y, Z;
#ifdef METHOD_RANDOM
            X = random_number(zero.x, max.x+1);
            Y = random_number(zero.y, max.y+1);
            Z = random_number(zero.z, max.z+1);
#else
            X = zero.x+x*resolution+resolution/2;
            Y = zero.y+y*resolution+resolution/2;
            Z = zero.z+z*resolution+resolution/2;
#endif
            //std::cout << X << " " << Y << " " << Z << std::endl;
            Line l(Point3(X,Y,Z), Point3(DX, DY, DZ));
            auto faces = octree.facesFor(l);
            for (auto face : faces) {
                float alpha;
                if (face->intersects(l, &alpha)) {
                    if (alpha > 0) {
                        right++;
                    } else {
                        left++;
                    }
                }
            }
            
            /*
            int oleft=0, oright=0;
            for (auto &volume : m.volumes)
            for (auto &face : volume.faces) {
                float alpha;
                if (face.intersects(l, &alpha)) {
                    if (alpha > 0) {
                        oright++;
                    } else {
                        oleft++;
                    }
                }
            }
            if (left!=oleft || right!=oright) {
                std::cout << "=========== ERROR =============" << std::endl;
                l.dump();
                std::cout << oleft << "/" << oright << std::endl;
                std::cout << left << "/" << right << std::endl;
                for (auto &face : faces) {
                //for (auto &volume : m.volumes)
                //for (auto &face : volume.faces) {
                    float alpha;
                    if (face->intersects(l, &alpha)) {
                        // auto p = l.get(alpha);
                        // std::cout << p.x << " " << p.y << " " << p.z << std::endl;
                        //face.dump();
                        face->dump();
//                        face->gnuplot();
                        std::cout << std::endl << std::endl;
                    }
                }
                exit(0);
            }
            */

            if (left%2 || right%2) {
                // Inside the part
                //std::cout << X << " " << Y << " " << Z << std::endl;

                // Increasing volume
                cubes++;

                // Updating center of mass
                com.x += X;
                com.y += Y;
                com.z += Z;

                // Updating inertia
                dynamics.ixx += (Y*Y + Z*Z);
                dynamics.iyy += (X*X + Z*Z);
                dynamics.izz += (X*X + Y*Y);
                dynamics.ixy -= X*Y;
                dynamics.iyz -= Y*Z;
                dynamics.ixz -= X*Z;
            }
        }

        double volume_per_cube = resolution*resolution*resolution;
        double mass_per_cube;

        if (mass > 0.0001) {
            // Mass is already set
            mass_per_cube = mass/cubes;
        } else {
            // Using density
            mass_per_cube = volume_per_cube*(density/1000.);
        }

        if (cubes) {
            dynamics.ixx *= mass_per_cube;
            dynamics.iyy *= mass_per_cube;
            dynamics.izz *= mass_per_cube;
            dynamics.ixy *= mass_per_cube;
            dynamics.iyz *= mass_per_cube;
            dynamics.ixz *= mass_per_cube;

            dynamics.volume = cubes*volume_per_cube;
            dynamics.mass = cubes*mass_per_cube;
            com.x /= cubes;
            com.y /= cubes;
            com.z /= cubes;
            dynamics.com.values[0] = com.x;
            dynamics.com.values[1] = com.y;
            dynamics.com.values[2] = com.z;
            dynamics.computed = true;
        }

        return dynamics;
    }
}
