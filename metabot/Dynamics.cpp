#include <iostream>
#include "Dynamics.h"

namespace Metabot
{
    Dynamics::Dynamics()
        : com(0, 0, 0), volume(0)
    {
    }
            
    Dynamics Dynamics::combine(const Dynamics &other, TransformMatrix matrix)
    {
        auto other_com = matrix.apply(other.com);
        Dynamics dynamics;
        std::cout << "Combining with " << other.volume << std::endl;
        double a = volume;
        double b = other.volume;
        double total = a+b;
        if (total > 0.00001) {
            dynamics.com.values[0] = (com.values[0]*a + other_com.values[0]*b)/total;
            dynamics.com.values[1] = (com.values[1]*a + other_com.values[1]*b)/total;
            dynamics.com.values[2] = (com.values[2]*a + other_com.values[2]*b)/total;
        }
        dynamics.volume = total;

        return dynamics;
    }
}
