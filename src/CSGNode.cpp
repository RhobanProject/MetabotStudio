#include <iostream>
#include "CSGNode.h"
#include "util.h"

namespace Metabot
{
    CSGNode::CSGNode(std::string name_, std::string value_)
        : name(name_), value(value_), anchor(false), model(false), part(false)
    {
        if (isMatrix()) {
            matrix = TransformMatrix::fromJSON(value);
        }

        if (isMarker()) {
            std::string noquote = value.substr(1, value.length()-2);

            if (startswith(noquote, "metabot_model: ")) {
                model = true;
                data = trim(removestart(noquote, "metabot_model: "));
            }
            if (startswith(noquote, "metabot_anchor: ")) {
                anchor = true;
                data = trim(removestart(noquote, "metabot_anchor: "));
            }
            if (startswith(noquote, "metabot_part: ")) {
                part = true;
                data = trim(removestart(noquote, "metabot_part: "));
            }
        }
    }

    bool CSGNode::isMatrix()
    {
        return (name == "multmatrix");
    }

    bool CSGNode::isMarker()
    {
        return (name == "marker");
    }
 
    CSGNode::~CSGNode()
    {
        for (auto node : children) {
            delete node;
        }
    }
}
