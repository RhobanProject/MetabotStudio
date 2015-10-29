#include <iostream>
#include "Kinematic.h"

namespace Metabot
{
    Kinematic::Kinematic()
        : alpha(0), tip(0)
    {
    }

    void Kinematic::display()
    {
        std::cout << "class RobotKinematic {" << std::endl;
        std::cout << "public:" << std::endl;
        std::cout << "  struct Point3D { double x, y, z; };" << std::endl;
        std::cout << "double alpha[" << alpha << "];" << std::endl;
        std::cout << code << std::endl;
        std::cout << "};" << std::endl;
    }

    std::string Kinematic::addJoint()
    {
        std::stringstream ss;
        ss << "alpha[" << alpha << "]";
        alpha++;

        return ss.str();
    }

    void Kinematic::addTip(Symbolic x, Symbolic y, Symbolic z, std::vector<Symbolic> dependances)
    {
        std::stringstream ss;
        ss << "RobotKinematic::Point3D tip_" << (tip++) << "_position() {" << std::endl;
        ss << "  RobotKinematic::Point3D point;" << std::endl;
        ss << "  point.x = " << x << ";" << std::endl;
        ss << "  point.y = " << y << ";" << std::endl;
        ss << "  point.z = " << z << ";" << std::endl;
        ss << "  return point;" << std::endl;
        ss << "}" << std::endl <<  std::endl;
        code += ss.str();

        Kinematic::Tip tip;
        tip.dependances = dependances;
        tip.x = x;
        tip.y = y;
        tip.z = z;
        tips.push_back(tip);
    }
}
