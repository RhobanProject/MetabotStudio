#include <iostream>
#include "Octree.h"

namespace Metabot
{
    Octree::Octree()
    {
    }

    void Octree::load(Model model)
    {
        auto min = model.min();
        auto max = model.max();

        root = OctreeFloor(Cube(min.x-1, max.x+1, min.y-1, max.y+1, min.z-1, max.z+1), 5);

        for (auto &volume : model.volumes) {
            for (auto &face : volume.faces) {
                root.add(face);
            }
        }
    }

    OctreeFloor::OctreeFloor()
        : splitted(false)
    {
    }

    OctreeFloor::OctreeFloor(Cube cube_, int depth_)
        : cube(cube_), depth(depth_), splitted(false)
    {
        cube.gnuplot();
    }

    void OctreeFloor::split()
    {
        splitted = true;
        double xMin = cube.xMin;
        double xMax = cube.xMax;
        double xMid = (xMin + xMax)/2;
        double yMin = cube.yMin;
        double yMax = cube.yMax;
        double yMid = (yMin + yMax)/2;
        double zMin = cube.zMin;
        double zMax = cube.zMax;
        double zMid = (zMin + zMax)/2;

        floors.push_back(OctreeFloor(Cube(xMin, xMid, yMin, yMid, zMin, zMid), depth-1));
        floors.push_back(OctreeFloor(Cube(xMid, xMax, yMin, yMid, zMin, zMid), depth-1));
        floors.push_back(OctreeFloor(Cube(xMin, xMid, yMid, yMax, zMin, zMid), depth-1));
        floors.push_back(OctreeFloor(Cube(xMid, xMax, yMid, yMax, zMin, zMid), depth-1));
        floors.push_back(OctreeFloor(Cube(xMin, xMid, yMin, yMid, zMid, zMax), depth-1));
        floors.push_back(OctreeFloor(Cube(xMid, xMax, yMin, yMid, zMid, zMax), depth-1));
        floors.push_back(OctreeFloor(Cube(xMin, xMid, yMid, yMax, zMid, zMax), depth-1));
        floors.push_back(OctreeFloor(Cube(xMid, xMax, yMid, yMax, zMid, zMax), depth-1));
    }
    
    void OctreeFloor::add(Face face)
    {
        if (depth > 0) {
            if (!splitted) {
                split();
            }

            std::vector<OctreeFloor*> containing;
            for (auto &floor : floors) {
                if (floor.cube.contains(face)) {
                    containing.push_back(&floor);
                }
            }

            if (containing.size() == 1) {
                containing[0]->add(face);
                return;
            }
        }

        faces.push_back(face);
    }
}
