#include <iostream>
#include <sstream>
#include "Shape.h"

#define RETRACTION 0.2

namespace Metabot
{
    std::string Shape::toURDF(TransformMatrix transform)
    {
        std::stringstream ss;
        ss << "    " << (transform.multiply(matrix)).toURDF() << std::endl;
        ss << "    <geometry>" << std::endl;
        if (type == SHAPE_BOX) {
            ss << "      <box size=\"" << ((a-RETRACTION)/1000) << " " << ((b-RETRACTION)/1000) << " " << ((c-RETRACTION)/1000) << "\"/>" << std::endl;
        } else if (type == SHAPE_SPHERE) {
            ss << "      <sphere radius=\"" << ((r-RETRACTION)/1000) << "\"/>" << std::endl;
        } else if (type == SHAPE_CYLINDER) {
            ss << "      <cylinder radius=\"" << ((r-RETRACTION)/1000) << "\" length=\"" << ((h-RETRACTION)/1000) << "\"/>" << std::endl;
        }
        ss << "    </geometry>";

        return ss.str();
    }

    std::string Shape::toSDF(TransformMatrix transform)
    {
        std::stringstream ss;
        ss << "    " << (transform.multiply(matrix)).toSDF() << std::endl;
        ss << "    <geometry>" << std::endl;
        if (type == SHAPE_BOX) {
            ss << "      <box><size>" << ((a-RETRACTION)/1000) << " " << ((b-RETRACTION)/1000) << " " << ((c-RETRACTION)/1000) << "</size></box>" << std::endl;
        } else if (type == SHAPE_SPHERE) {
            ss << "      <sphere><radius>" << ((r-RETRACTION)/1000) << "</radius></sphere>" << std::endl;
        } else if (type == SHAPE_CYLINDER) {
            ss << "      <cylinder><radius>" << ((r-RETRACTION)/1000) << "</radius><length>" << ((h-RETRACTION)/1000) << "</length></cylinder>" << std::endl;
        }
        ss << "    </geometry>";

        return ss.str();
    }
}
