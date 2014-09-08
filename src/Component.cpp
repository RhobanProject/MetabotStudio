#include <iostream>
#include <fstream>
#include "util.h"
#include "Component.h"
#include "ComponentInstance.h"

namespace Metabot
{
    Component::Component(std::string name_, std::string filename_)
        : name(name_), filename(filename_), type("")
    {
    }
            
    Component::~Component()
    {
        for (auto parameter : parameters) {
            delete parameter.second;
        }
    }
    
    ComponentInstance *Component::instanciate()
    {
        ComponentInstance *instance = new ComponentInstance(this);
        for (auto parameter : parameters) {
            instance->values[parameter.first] = parameter.second->value;
        }

        return instance;
    }

    Component *Component::load(std::string filename)
    {
        bool isOk = false;
        std::ifstream f(filename);
        std::string line;
        std::string description;
        int state = 0;
        std::string name = basename(filename);
        Component *component = new Component(name, filename);

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
                        ComponentParameter *parameter = new ComponentParameter(param, value, description);
                        component->parameters[parameter->name] = parameter;
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
