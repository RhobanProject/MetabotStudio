#pragma once

#include "Point3.h"
#include "Line.h"
#include "Face.h"

namespace Metabot
{
    class Cube
    {
        public:
            Cube();
            Cube(double xMin, double xMax,
                 double yMin, double yMax, 
                 double zMin, double zMax);

            bool contains(Point3 point);
            bool contains(Face face);
            bool intersects(Line line);
            void gnuplot();
            
            double xMin, xMax, yMin, yMax, zMin, zMax;
    };
}
