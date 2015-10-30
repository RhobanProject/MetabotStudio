#include <iostream>
#include "Kinematic.h"

namespace Metabot
{
    void Kinematic::Chain::addMatrix(TransformMatrix matrix)
    {
        Kinematic::ChainItem item;
        item.type = CHAIN_MATRIX;
        item.matrix = matrix;
        items.push_back(item);
    }

    void Kinematic::Chain::addRotation(int alpha)
    {
        Kinematic::ChainItem item;
        item.type = CHAIN_ROTATION;
        item.alpha = alpha;
        items.push_back(item);
        angles[alpha] = 0.0;
    }
                    
    Point3 Kinematic::Chain::position()
    {
        auto matrix = TransformMatrix::identity();

        for (auto item : items) {
            if (item.type == CHAIN_MATRIX) {
                matrix = matrix.multiply(item.matrix);
            } else if (item.type == CHAIN_ROTATION) {
                matrix = matrix.multiply(TransformMatrix::rotationZ(angles[item.alpha]));
            }
        }

        return Point3(matrix.x(), matrix.y(), matrix.z());
    }

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

    void Kinematic::addTip(Symbolic x, Symbolic y, Symbolic z, Chain chain)
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
        tip.x = x;
        tip.y = y;
        tip.z = z;
        tip.chain = chain;
        tips.push_back(tip);
    }
}
