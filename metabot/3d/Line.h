#pragma once

#include "Point3.h"

namespace Metabot
{
    class Line
    {
        public:
            Line(Point3 origin, Point3 direction);
            static Line fromTo(Point3 origin, Point3 destination);
            Point3 get(float alpha);
            void gnuplot(bool segment=false);

            Point3 origin;
            Point3 direction;
    };
}
