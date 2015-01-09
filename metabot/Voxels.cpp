#include <iostream>
#include <vector>
#include <3d/Octree.h>
#include "Voxels.h"

#define DX  0.8170638283522394
#define DY -0.2298170729638312
#define DZ  1.8270672983620736

namespace Metabot
{
    Voxels::Voxels(int width, int height, int depth)
    {
        points.resize(width*height*depth);
    }

    void Voxels::voxelize(Model m)
    {
        float resolution = 1.0;

        // Zero point
        auto zero = m.min();
        auto max = m.max();
        // Dimensions
        int width = (max.x+1-zero.x)/resolution;
        int height = (max.y+1-zero.y)/resolution;
        int depth = (max.z+1-zero.z)/resolution;

        Octree octree;
        octree.load(m);

        for (int z=0; z<depth; z++) 
        for (int y=0; y<height; y++)
        for (int x=0; x<width; x++)
        {
            int left = 0;
            int right = 0;
            float X, Y, Z;
            X = zero.x+x*resolution+resolution/2;
            Y = zero.y+y*resolution+resolution/2;
            Z = zero.z+z*resolution+resolution/2;
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

            if (left%2 && right%2) {
                std::cout << X << " " << Y << " " << Z << std::endl;
            }
        }
    }
}
