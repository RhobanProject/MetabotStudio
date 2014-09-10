#include "ModelRef.h"

namespace Metabot
{
    ModelRef::ModelRef(Json::Value data, TransformMatrix matrix_)
        : name(""), matrix(matrix_), r(0.6), g(0.6), b(0.6)
    {
        if (data.isObject()) {
            if (data.isMember("name")) {
                name = data["name"].asString();
            }
            if (data.isMember("color") && data["color"].isArray()
                    && data["color"].size() == 3) {
                r = data["color"][0].asFloat();
                g = data["color"][1].asFloat();
                b = data["color"][2].asFloat();
            }
        }
    }
}
