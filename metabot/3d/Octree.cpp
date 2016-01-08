#include <iostream>
#include "Octree.h"

namespace Metabot
{
    Octree::Octree()
    {
    }

    Octree::~Octree()
    {
        for (auto face : faces) {
            delete face;
        }
        faces.clear();
    }

    void Octree::load(Model model)
    {
        auto min = model.min();
        auto max = model.max();

        root = OctreeFloor(Cube(min.x-1, max.x+1, min.y-1, max.y+1, min.z-1, max.z+1), 4);

        //add(Face(Point3(17.2,7.73939,8.73626),Point3(17.2,7.48578,8.44599),Point3(17.2,45,-9.99201e-15)));
        //return;
        for (auto &volume : model.volumes) {
            for (auto &face : volume.faces) {
                add(face);
            }
        }
    }

    void Octree::add(Face face)
    {
        Face *f = new Face;
        *f = face;
        faces.push_back(f);
        root.add(f);
    }
            
    std::set<Face*> Octree::facesFor(Line line)
    {
        std::set<Face*> faces;
        root.facesFor(faces, line);

        return faces;
    }

    OctreeFloor::OctreeFloor()
        : splitted(false)
    {
    }

    OctreeFloor::OctreeFloor(Cube cube_, int depth_)
        : cube(cube_), depth(depth_), splitted(false)
    {
        // cube.gnuplot();
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
    
    void OctreeFloor::add(Face *face)
    {
        if (depth > 0) {
            if (!splitted) {
                split();
            }

            int containing = 0;
            // std::vector<OctreeFloor*> containing;
            for (auto &floor : floors) {
                if (floor.cube.intersects(*face)) {
                    containing++;
                    floor.add(face);
                    // containing.push_back(&floor);
                }
            }

            // if (containing.size() == 1) {
            //    containing[0]->add(face);
            if (containing > 0) {
                return;
            }
        }

        faces.push_back(face);
    }
            
    void OctreeFloor::facesFor(std::set<Face*> &result, Line line)
    {
        if (cube.intersects(line)) {
            // cube.gnuplot();
            for (auto &face : faces) {
                result.insert(face);
            }

            for (auto &floor : floors) {
                floor.facesFor(result, line);
            }
        }
    }
}
