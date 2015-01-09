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
            bool intersects(Line line, double *alpha_1=NULL, double *alpha_2=NULL);
            bool intersectsSegment(Line line);
            bool intersects(Face face, bool debug=false);
            void gnuplot();
            
            double xMin, xMax, yMin, yMax, zMin, zMax;
    };
}
