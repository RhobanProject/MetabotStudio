#include <iostream>
#include "Line.h"

namespace Metabot
{
    Line::Line(Point3 origin_, Point3 direction_)
        : origin(origin_), direction(direction_)
    {
    }
            
    Point3 Line::get(float alpha)
    {
        return Point3(origin.x+alpha*direction.x, origin.y+alpha*direction.y, 
                origin.z+alpha*direction.z);
    }

    void Line::gnuplot()
    {
        std::cout << origin.x << " " << origin.y << " " << origin.z << std::endl;
        for (float alpha=1; alpha<5; alpha+=0.1) {
            auto p = get(alpha);
            std::cout << p.x << " " << p.y << " " << p.z << std::endl;
        }
    }
}
