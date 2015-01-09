#include <iostream>
#include <matrix.h>
#include "Face.h"

namespace Metabot
{
	Face::Face()
    {
    }

    Face::~Face()
    {
    }

    Face::Face(Point3 v0, Point3 v1, Point3 v2)
    {
    	v[0] = v0;
    	v[1] = v1;
    	v[2] = v2;
    }
            
    bool Face::intersects(const Line &l, float *alpha)
    {
        double3x3 mat;

        mat[0][0] = l.direction.x;
        mat[1][0] = l.direction.y;
        mat[2][0] = l.direction.z;

        mat[0][1] = -(v[1].x-v[0].x);
        mat[1][1] = -(v[1].y-v[0].y);
        mat[2][1] = -(v[1].z-v[0].z);
        
        mat[0][2] = -(v[2].x-v[0].x);
        mat[1][2] = -(v[2].y-v[0].y);
        mat[2][2] = -(v[2].z-v[0].z);

        double det;
        double3x3 inv;
        INVERT_3X3(inv, det, mat);

        double3 point;
        point[0] = v[0].x-l.origin.x;
        point[1] = v[0].y-l.origin.y;
        point[2] = v[0].z-l.origin.z;

        double3 result;
        MAT_DOT_VEC_3X3(result, inv, point);

        if (alpha != NULL) {
            *alpha = result[0];
        }
        double a = result[1];
        double b = result[2];

        return (a>0 && b>0 && (a+b)<1);
    }
            
    bool Face::intersectsSegment(const Line &l, float *alpha)
    {
        float a;
        bool i = intersects(l, &a);

        if (alpha != NULL) {
            *alpha = a;
        }

        return (i && a>=0 && a<=1);
    }

    void Face::gnuplot()
    {
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << v[0].x << " " << v[0].y << " " << v[0].z << std::endl;
        std::cout << v[1].x << " " << v[1].y << " " << v[1].z << std::endl;
        std::cout << v[2].x << " " << v[2].y << " " << v[2].z << std::endl;
        std::cout << v[0].x << " " << v[0].y << " " << v[0].z << std::endl;
    }

    void Face::dump()
    {
        std::cout << "Face(";
        std::cout << "Point3(" << v[0].x << "," << v[0].y << "," << v[0].z << "),";
        std::cout << "Point3(" << v[1].x << "," << v[1].y << "," << v[1].z << "),";
        std::cout << "Point3(" << v[2].x << "," << v[2].y << "," << v[2].z << ")";
        std::cout << ")" << std::endl;
    }
}
