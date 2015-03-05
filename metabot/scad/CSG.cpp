#include <iostream>
#include <sstream>
#include "CSG.h"
#include "CSGNode.h"
#include "util.h"

namespace Metabot
{
    CSG::CSG()
    {
        root = new CSGNode("root");
    }

    CSG::~CSG()
    {
        if (root != NULL) {
            delete root;
        }
    }

    CSG CSG::parse(std::string data)
    {
        CSG document;
        int state = 0;
        unsigned int n = data.length();
        std::vector<CSGNode *> stack;
        std::string name;
        std::string value;
        stack.push_back(document.root);

        for (unsigned int i=0; i<n; i++) {
            char c = data[i];
            switch (state) {
                case 0:
                    if (c == '}') {
                        if (stack.size() == 1) {
                            throw std::string("CSG: Too much }");
                        }
                        stack.pop_back();
                    } else if (c == '(') {
                        state++;
                    } else {
                        name += c;
                    }
                break;
                case 1:
                    if (c == ')') {
                        state = 3;
                    } else {
                        value += c;
                        if (c == '"') {
                            state = 2;
                        }
                    }
                break;
                case 2:
                    value += c;
                    if (c == '"') {
                        state = 1;
                    }
                break;
                case 3:
                    if (c == ';' || c == '{') {
                        name = trim(name);
                        value = trim(value);
                        CSGNode *node = new CSGNode(name, value);
                        CSGNode *last = stack[stack.size()-1];
                        last->children.push_back(node);

                        if (c == '{') { 
                            stack.push_back(node);
                        }

                        name = "";
                        value = "";
                        state = 0;
                    }
                break;
            }
        }

        if (stack.size() != 1) {
            throw std::string("CSG: No { } matching");
        }

        auto matrix = TransformMatrix::identity();
        document.walk(matrix, document.root);

        return document;
    }
            
    void CSG::walk(TransformMatrix matrix, CSGNode *node)
    {
        if (node->isMatrix()) {
            matrix = matrix.multiply(node->matrix);
        }

        if (node->isShape()) {
            shapes.push_back(node->toShape(matrix));
        }

        if (node->is("anchor")) {
            AnchorPoint *anchor = new AnchorPoint(node->json, matrix);
            anchors.push_back(anchor);
        }

        if (node->is("model")) {
            models.add(Ref(node->json, matrix));
        }

        if (node->is("part")) {
            parts.add(Ref(node->json, matrix));
        }

        if (node->is("bom")) {
            bom.append(BOMEntry(node->json));
        }

        if (node->is("contact")) {
            contacts.push_back(matrix);
        }

        for (auto child : node->children) {
            walk(matrix, child);
        }
    }
}
