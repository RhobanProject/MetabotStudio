#include <iostream>
#include "Cube.h"

namespace Metabot
{
    Cube::Cube()
    {
    }

    Cube::Cube(double xMin_, double xMax_,
                 double yMin_, double yMax_, 
                 double zMin_, double zMax_)
        : xMin(xMin_), xMax(xMax_), yMin(yMin_), 
        yMax(yMax_), zMin(zMin_), zMax(zMax_)
    {
    }

    bool Cube::contains(Point3 point)
    {
        return ((point.x >= xMin && point.x <= xMax) &&
                (point.y >= yMin && point.y <= yMax) &&
                (point.z >= zMin && point.z <= zMax));
    }

    static void consider(double vmin, double vmax, double start, double variation, 
            double &minimum, double &maximum, bool force=false)
    {
        double lower, upper;
        if (variation > 0) {
            lower = (vmin-start)/variation;
            upper = (vmax-start)/variation;
        } else {
            lower = (vmax-start)/variation;
            upper = (vmin-start)/variation;
        }

        if ((force) || (lower > minimum)) minimum = lower;
        if ((force) || (upper < maximum)) maximum = upper;
    }

    bool Cube::intersects(Line line)
    {
        double minimum, maximum;
        consider(xMin, xMax, line.origin.x, line.direction.x, minimum, maximum, true);
        consider(yMin, yMax, line.origin.y, line.direction.y, minimum, maximum);
        consider(zMin, zMax, line.origin.z, line.direction.z, minimum, maximum);

        /**
        // Plots the intersection
        auto A = line.get(minimum);
        auto B = line.get(maximum);
        std::cout << A.x << " " << A.y << " " << A.z << std::endl;
        std::cout << B.x << " " << B.y << " " << B.z << std::endl;
        */

        return (minimum < maximum);
    }
    
    bool Cube::contains(Face face)
    {
        return contains(face.v[0]) && contains(face.v[1]) && contains(face.v[2]);
    }

    void Cube::gnuplot()
    {
        // Front face
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << xMin << " " << yMin << " " << zMin << std::endl;
        std::cout << xMax << " " << yMin << " " << zMin << std::endl;
        std::cout << xMax << " " << yMax << " " << zMin << std::endl;
        std::cout << xMin << " " << yMax << " " << zMin << std::endl;
        std::cout << xMin << " " << yMin << " " << zMin << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
        // Back face
        std::cout << xMin << " " << yMin << " " << zMax << std::endl;
        std::cout << xMax << " " << yMin << " " << zMax << std::endl;
        std::cout << xMax << " " << yMax << " " << zMax << std::endl;
        std::cout << xMin << " " << yMax << " " << zMax << std::endl;
        std::cout << xMin << " " << yMin << " " << zMax << std::endl;
        // Links
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << xMin << " " << yMin << " " << zMin << std::endl;
        std::cout << xMin << " " << yMin << " " << zMax << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << xMax << " " << yMin << " " << zMin << std::endl;
        std::cout << xMax << " " << yMin << " " << zMax << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << xMin << " " << yMax << " " << zMin << std::endl;
        std::cout << xMin << " " << yMax << " " << zMax << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << xMax << " " << yMax << " " << zMin << std::endl;
        std::cout << xMax << " " << yMax << " " << zMax << std::endl;
    }
}
