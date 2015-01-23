#include <iostream>
#include <json/json.h>
#include "CSGNode.h"
#include "Shape.h"
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
        std::string buffer="", out="";

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

    bool CSGNode::isShape()
    {
        return (name == "cube" || name == "cylinder" || name == "sphere");
    }

    Shape CSGNode::toShape(TransformMatrix m)
    {
        Shape shape;
        auto vars = getVars(value);
        shape.matrix = m;

        if (name == "cube") {
            shape.type = SHAPE_BOX;
            auto size = splitCSV(vars["size"].substr(1, vars["size"].length()-1));
            if (size.size() == 3) {
                shape.a = atof(size[0].c_str());
                shape.b = atof(size[1].c_str());
                shape.c = atof(size[2].c_str());
            }
            if (vars["center"]=="false") {
                shape.matrix = m.multiply(TransformMatrix::translation(shape.a/2, shape.b/2, shape.c/2));
            }
        }
        if (name == "sphere") {
            shape.type = SHAPE_SPHERE;
            shape.r = atof(vars["r"].c_str());
        }
        if (name == "cylinder") {
            shape.type = SHAPE_CYLINDER;
            shape.r = atof(vars["r1"].c_str());
            shape.h = atof(vars["h"].c_str());
            if (vars["center"]=="false") {
                shape.matrix = m.multiply(TransformMatrix::translation(0, 0, shape.h/2));
            }
        }

        return shape;
    }
}
