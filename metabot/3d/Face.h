#pragma once

#include <stdio.h>
#include "Point3.h"
#include "Line.h"

namespace Metabot
{
    class Face
    {
        public:
            virtual ~Face();

            bool intersects(const Line &l, float *alpha=NULL);
            bool intersectsSegment(const Line &l, float *alpha=NULL);
            void gnuplot();

			Point3 v[3];
			Face();
			Face(Point3 v0, Point3 v1, Point3 v2);
    };
}
