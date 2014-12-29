#include <iostream>
#include <json/json.h>
#include "CSGNode.h"
#include "util.h"

namespace Metabot
{
    CSGNode::CSGNode(std::string name_, std::string value_)
        : name(name_), value(value_)
    {
        if (isMatrix()) {
            matrix = TransformMatrix::fromJSON(value);
        }

        if (isMarker()) {
            std::string noquote = value.substr(1, value.length()-1);

            if (startswith(noquote, "metabot: ")) {
                process(removestart(noquote, "metabot: "));
            }
        }
    }

    void CSGNode::process(std::string data)
    {
        data = fix(data);

        Json::Reader reader;
        reader.parse(data, json);
    }

    std::string CSGNode::fix(std::string data)
    {
        bool in_enclosure = true;
        std::string buffer, out;

        for (unsigned int i=0; i<data.size(); i++) {
            char c = data[i];
            if (c == '"') {
                if (!in_enclosure) {
                    out += trim(buffer);
                }
                in_enclosure = !in_enclosure;
                buffer = "";
            } else {
                if (in_enclosure) {
                    out += c;
                } else {
                    if (c != ',') {
                        buffer += c;
                    }
                }
            }
        }

        for (unsigned int i=0; i<out.size(); i++) {
            if (out[i] == '\'') {
                out[i] = '"';
            }
        }

        return out;
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
            
    bool CSGNode::is(std::string type)
    {
        return json["type"]==type;
    }
}
