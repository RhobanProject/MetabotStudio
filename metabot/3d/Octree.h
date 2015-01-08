#pragma once

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
            void add(Face face);

            std::vector<Face> faces;
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

        protected:
            OctreeFloor root;
    };
}
