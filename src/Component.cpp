#include <iostream>
#include <fstream>
#include "util.h"
#include "Component.h"

namespace Metabot
{
    Component::Component(std::string name_)
        : name(name_), type("")
    {
    }
            
    Component::~Component()
    {
        for (auto parameter : parameters) {
            delete parameter;
        }
    }

    Component *Component::load(std::string filename)
    {
        bool isOk = false;
        std::ifstream f(filename);
        std::string line;
        std::string description;
        int state = 0;
        std::string name = basename(filename);
        Component *component = new Component(name);

        while (std::getline(f, line)) {
            switch (state) {
                case 0: {
                    if (line.substr(0, 12) == "// Component") {
                        isOk = true;
                        component->type = trim(line.substr(12));
                    } else if (line.substr(0, 12) == "// Parameter") {
                        description = trim(line.substr(12));
                        state = 1;
                    }
                }
                break;
                case 1: {
                    std::vector<std::string> parts = split(line, '=');
                    if (parts.size() == 2) {
                        std::string param = trim(parts[0]);
                        std::string value = trim(parts[1]);
                        if (value[value.length()-1] == ';') {
                            value = value.substr(0, value.length()-1);
                        }
                        component->parameters.push_back(new ComponentParameter(name, value, description));
                    }
                    state = 0;
                }
                break;
            }
        }

        if (isOk) {
            return component;
        } else {
            delete component;
            return NULL;
        }
    }
}
