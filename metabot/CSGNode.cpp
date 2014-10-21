#include <iostream>
#include <json/json.h>
#include "CSGNode.h"
#include "util.h"

namespace Metabot
{
    CSGNode::CSGNode(std::string name_, std::string value_)
        : name(name_), value(value_), anchor(false), model(false), part(false), parameter(false), bom(false)
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
            if (startswith(noquote, "metabot_parameter: ")) {
                parameter = true;
                data = trim(removestart(noquote, "metabot_parameter: "));
            }
            if (startswith(noquote, "metabot_bom: ")) {
                bom = true;
                data = trim(removestart(noquote, "metabot_bom: "));
            }
        }
    }
    
    CSGNode::~CSGNode()
    {
        for (auto node : children) {
            delete node;
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

    Json::Value CSGNode::json()
    {
        std::string doc = data;
        Json::Value value;
        Json::Reader reader;
        for (unsigned int i=0; i<doc.length(); i++) {
            if (doc[i] == '\'') doc[i] = '"';
        }
        reader.parse(doc, value);
        // std::cout << doc << std::endl;
        // std::cout << value << std::endl;
        return value;
    }
}
