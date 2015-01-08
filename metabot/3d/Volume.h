#pragma once

#include "Face.h"
#include <vector>
using std::vector;

namespace Metabot
{
    class Volume
    {
        public:
            virtual ~Volume();
			vector<Face> faces;
			Volume();
			void addFace(Face f);
			Point3 min();
			Point3 max();
    };
}
