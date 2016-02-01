#include <math.h>
#include "Point3.h"

namespace Metabot
{
	Point3::Point3()
    {
    }

    Point3::Point3(double x, double y, double z)
        : x(x), y(y), z(z)
    {
    }
            
    double Point3::norm()
    {
        return sqrt(x*x + y*y + z*z);
    }
            
    double Point3::distance(const Point3 &other)
    {
        return sqrt(pow(x-other.x, 2) + pow(y-other.y, 2) + pow(z-other.z, 2));
    }
}
