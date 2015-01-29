#include <iostream>
#include <sstream>
#include "Dynamics.h"

namespace Metabot
{
    Dynamics::Dynamics()
        : com(0, 0, 0), volume(0), mass(0), computed(false)
    {
    }
            
    void Dynamics::combine(const Dynamics &other, TransformMatrix matrix)
    {
        if (computed) {
            if (other.computed) {
                auto other_com = matrix.apply(other.com);
                double a = mass;
                double b = other.mass;
                double total = a+b;
                if (total > 0.00001) {
                    com.values[0] = (com.x()*a + other_com.x()*b)/total;
                    com.values[1] = (com.y()*a + other_com.y()*b)/total;
                    com.values[2] = (com.z()*a + other_com.z()*b)/total;
                }
                volume = volume+other.volume;
                mass = total;
                computed = true;
            }
        } else {
            if (other.computed) {
                computed = true;
                com = matrix.apply(other.com);
                mass = other.mass;
                volume = other.volume;
            }
        }
    }

    std::string Dynamics::toString()
    {
        std::stringstream ss;
        if (computed) {
            ss << "Volume: " << volume << "mm^3" << std::endl;
            ss << "Mass: " << mass << "g" << std::endl;
            ss << "COM: " << com.x() << ", " << com.y() << ", " << com.z() << std::endl;
        } else {
            ss << "No dynamics (no geometry or not computed)" << std::endl;
        }

        return ss.str();
    }
}
