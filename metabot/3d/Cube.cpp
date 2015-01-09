#include <vector>
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

    bool Cube::intersects(Line line, double *alpha_1, double *alpha_2)
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
        if (alpha_1 != NULL) {
            *alpha_1 = minimum;
        }
        if (alpha_2 != NULL) {
            *alpha_2 = maximum;
        }

        return (minimum <= maximum);
    }

    bool Cube::intersectsSegment(Line line)
    {
        double a, b;
        bool i = intersects(line, &a, &b);

        return (i && a<=1 && b>=0);
    }
            
    bool Cube::intersects(Face face, bool debug)
    {
        // If one of the vertexes is in the cube
        if (contains(face.v[0]) || contains(face.v[1]) || contains(face.v[2])) {
            return true;
        }

        // The cube intersects one of the vertex of the face
        auto fa = Line::fromTo(face.v[0], face.v[1]);
        auto fb = Line::fromTo(face.v[1], face.v[2]);
        auto fc = Line::fromTo(face.v[2], face.v[0]);

        if (intersectsSegment(fa) || intersectsSegment(fb) || intersectsSegment(fc)) {
            return true;
        }

        // Else, the cube wheter intersects none of the facets, or it's "contained"
        // in the facet
        std::vector<Point3> points = {
            Point3(xMin, yMin, zMin), Point3(xMax, yMin, zMin), Point3(xMax, yMax, zMin), Point3(xMin, yMax, zMin),
            Point3(xMin, yMin, zMax), Point3(xMax, yMin, zMax), Point3(xMax, yMax, zMax), Point3(xMin, yMax, zMax)
        };
        std::vector<std::pair<Point3, Point3>> lines = {
            {points[0], points[1]}, {points[1], points[2]}, {points[2], points[3]}, {points[3], points[0]},
            {points[4], points[5]}, {points[5], points[6]}, {points[6], points[7]}, {points[7], points[4]},
            {points[0], points[4]}, {points[1], points[5]}, {points[2], points[6]}, {points[3], points[7]}
        };

        for (auto &line : lines) {
            auto l = Line::fromTo(line.first, line.second);
            if (face.intersectsSegment(l)) {
                return true;
            }
        }

        return false;
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
