#include "ModelRef.h"

namespace Metabot
{
    ModelRef::ModelRef(Json::Value json, TransformMatrix matrix_)
        : name(""), matrix(matrix_), r(0.6), g(0.6), b(0.6)
    {
        if (json.isObject()) {
            if (json.isMember("name")) {
                name = json["name"].asString();
            }
            if (json.isMember("color") && json["color"].isArray()
                    && json["color"].size() == 3) {
                r = json["color"][0].asFloat();
                g = json["color"][1].asFloat();
                b = json["color"][2].asFloat();
            }
        }
    }
}
