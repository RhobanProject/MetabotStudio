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
}
