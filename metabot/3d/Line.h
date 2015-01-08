#pragma once

#include "Point3.h"

namespace Metabot
{
    class Line
    {
        public:
            Line(Point3 origin, Point3 direction);
            Point3 get(float alpha);
            void gnuplot();

            Point3 origin;
            Point3 direction;
    };
}
