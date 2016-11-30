#include <iostream>
#include "Kinematic.h"

namespace Metabot
{
    void Kinematic::Tip::addMatrix(TransformMatrix matrix)
    {
        Kinematic::ChainItem item;
        item.type = CHAIN_MATRIX;
        item.matrix = matrix;
        chain.push_back(item);
    }

    void Kinematic::Tip::addRotation(std::string id, int jointId, float min, float max, int sign)
    {
        Kinematic::ChainItem item;
        item.type = CHAIN_ROTATION;
        item.id = id;
        item.min = min;
        item.max = max;
        item.sign = sign;
        item.jointId = jointId;
        chain.push_back(item);
    }
    
    std::vector<Point3> Kinematic::Tip::positions(const std::vector<double> &alphas)
    {
        std::vector<Point3> positions;
        auto matrix = TransformMatrix::identity();
        int k=0;

        for (auto item : chain) {
            if (item.type == CHAIN_MATRIX) {
                matrix = matrix.multiply(item.matrix);
            } else if (item.type == CHAIN_ROTATION) {
                matrix = matrix.multiply(TransformMatrix::rotationZ(item.sign*alphas[k++]));
            }
        
            positions.push_back(Point3(matrix.x(), matrix.y(), matrix.z()));
        }

        return positions;
    }
                    
    Point3 Kinematic::Tip::position(std::vector<double> &alphas)
    {
        auto matrix = TransformMatrix::identity();

        int k = 0;
        for (auto &item : chain) {
            if (item.type == CHAIN_MATRIX) {
                matrix = matrix.multiply(item.matrix);
            } else if (item.type == CHAIN_ROTATION) {
                auto alpha = alphas[k];
                if (alpha < item.min) alpha = item.min+0.05;
                if (alpha > item.max) alpha = item.max-0.05;
                matrix = matrix.multiply(TransformMatrix::rotationZ(item.sign*alpha));
                alphas[k] = alpha;
                k++;
            }
        }

        return Point3(matrix.x(), matrix.y(), matrix.z());
    }

    Kinematic::Kinematic()
        : alphaId(0), tipId(0)
    {
    }

    void Kinematic::display()
    {
        std::cout << "class RobotKinematic {" << std::endl;
        std::cout << "public:" << std::endl;
        std::cout << "  struct Point3D { double x, y, z; };" << std::endl;
        std::cout << "double alpha[" << alphaId << "];" << std::endl;
        std::cout << code << std::endl;
        std::cout << "};" << std::endl;
    }

    std::string Kinematic::addJoint()
    {
        std::stringstream ss;
        ss << "alpha[" << alphaId << "]";
        alphaId++;

        return ss.str();
    }

    void Kinematic::addTip(Tip tip)
    {
        std::stringstream ss;
        ss << "RobotKinematic::Point3D tip_" << (tipId++) << "_position() {" << std::endl;
        ss << "  RobotKinematic::Point3D point;" << std::endl;
        ss << "  point.x = " << *tip.x << ";" << std::endl;
        ss << "  point.y = " << *tip.y << ";" << std::endl;
        ss << "  point.z = " << *tip.z << ";" << std::endl;
        ss << "  return point;" << std::endl;
        ss << "}" << std::endl <<  std::endl;
        code += ss.str();

        tips.push_back(tip);
    }
}
