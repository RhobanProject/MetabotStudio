#include <iostream>
#include "Line.h"

namespace Metabot
{
    Line::Line(Point3 origin_, Point3 direction_)
        : origin(origin_), direction(direction_)
    {
    }
            
    Line Line::fromTo(Point3 origin, Point3 destination)
    {
        return Line(origin, Point3(destination.x-origin.x, destination.y-origin.y, destination.z-origin.z));
    }
            
    Point3 Line::get(float alpha)
    {
        return Point3(origin.x+alpha*direction.x, origin.y+alpha*direction.y, 
                origin.z+alpha*direction.z);
    }

    void Line::gnuplot(bool segment)
    {
        std::cout << origin.x << " " << origin.y << " " << origin.z << std::endl;
        float min = -50;
        float max = 50;
        if (segment) {
            min = 0;
            max = 1.01;
        }
        for (float alpha=min; alpha<max; alpha+=0.1) {
            auto p = get(alpha);
            std::cout << p.x << " " << p.y << " " << p.z << std::endl;
        }
    }
}
