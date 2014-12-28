#include <iostream>
#include <sstream>
#include <fstream>
#include "util.h"
#include "SCAD.h"

#define PARSER_NORMAL           0
#define PARSER_TAGGED           1
#define PARSER_MODULE           2

namespace Metabot
{
    std::vector<Module> SCAD::load(std::string filename)
    {
        std::string filename_out = filename.substr(0, filename.length()-5) + ".metabot.scad";
        std::ostringstream oss;
        oss << "NoModels = false;" << std::endl;
        
        // Modules found
        std::vector<Module> modules;
        // Current module
        Module module(filename_out);

        // Parser variables
        int state = PARSER_NORMAL;
        int level = 0;
        std::ifstream f(filename);
        std::string line;

        while (std::getline(f, line)) {
            auto tmp = trim(line);
            line = replaceStr(tmp, ".scad", ".metabot.scad");

            switch (state) {
                case PARSER_NORMAL:
                case PARSER_TAGGED:
                    if (startswith(line, "//:")) {
                        line = line.substr(3);
                        auto parts = split(line, ' ');
                        std::string annotation = strtolower(parts[0]);

                        if (annotation == "model" || annotation == "part" || annotation == "component") {
                            module.setType(annotation);
                            state = PARSER_TAGGED;
                        }
                        if (annotation == "description") {
                            module.setDescription(line.substr(12)); 
                        }
                    } else {
                        oss << line << std::endl;

                        if (startswith(line, "module") && state == PARSER_TAGGED) {
                            oss << module.pushLine(line);
                            state = PARSER_MODULE;
                        }
                    }
                break;
                case PARSER_MODULE:
                    oss << line << std::endl;
                    oss << module.pushLine(line);
                    if (module.finished()) {
                        state = PARSER_NORMAL;
                        modules.push_back(module);
                        module = Module(filename_out);
                    }
                break;
            }
        }

        file_put_contents(filename_out, oss.str());
        /*
        bool isOk = false;
        std::string description;
        int state = 0;
        std::string name = basename(filename);
        Component *component = new Component(name, filename);

        while (std::getline(f, line)) {
            switch (state) {
                case 0: {
                            if (line.substr(0, 12) == "// Component") {
                                isOk = true;
                                std::string value = trim(line.substr(12));
                                if (value != "") {
                                    component->prettyName = value;
                                }
                            } else if (line.substr(0, 14) == "// Description") {
                                component->description = trim(line.substr(14));
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
        */

        return modules;
    }
}
