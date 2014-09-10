#include <sstream>
#include "Vector.h"

namespace Metabot
{
    Vector::Vector()
    {
    }

    Vector::Vector(float x, float y, float z)
    {
        values[0] = x;
        values[1] = y;
        values[2] = z;
        values[3] = 1;
    }

    std::string Vector::toString()
    {
        std::stringstream str;

        for (int i=0; i<4; i++) {
            str << values[i] << std::endl;
        }

        return str.str();
    }
}
