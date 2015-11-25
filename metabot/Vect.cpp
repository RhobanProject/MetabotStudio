#include <sstream>
#include <cmath>
#include "Vect.h"

namespace Metabot
{
    Vect::Vect()
    {
        values[3] = 1;
    }

    Vect::Vect(float x, float y, float z)
    {
        values[0] = x;
        values[1] = y;
        values[2] = z;
        values[3] = 1;
    }

    std::string Vect::toString() const
    {
        std::stringstream str;

        for (int i=0; i<4; i++) {
            str << values[i] << std::endl;
        }

        return str.str();
    }
            
    float Vect::distance(const Vect &other)
    {
        float s = 0;
        for (int i=0; i<3; i++) {
            s += pow(other.values[i]-values[i],2);
        }

        return sqrt(s);
    }

    float Vect::x() const
    {
        return values[0];
    }

    float Vect::y() const
    {
        return values[1];
    }

    float Vect::z() const
    {
        return values[2];
    }
}
