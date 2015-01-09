#pragma once

#include <vector>
#include <set>
#include "Cube.h"
#include "Face.h"
#include "Model.h"

namespace Metabot
{
    class OctreeFloor
    {
        public:
            OctreeFloor();
            OctreeFloor(Cube cube, int depth);
            void add(Face *face);
            void facesFor(std::set<Face*> &faces, Line line);

            std::vector<Face*> faces;
            Cube cube;
            bool splitted;
            int depth;
            std::vector<OctreeFloor> floors;

        protected:
            void split();
    };

    class Octree
    {
        public:
            Octree();
            void load(Model model);
            void add(Face face);
            std::set<Face*> facesFor(Line line);

        protected:
            std::vector<Face*> faces;
            OctreeFloor root;
    };
}
