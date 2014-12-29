#include <sstream>
#include <iostream>
#include "Module.h"
#include "util.h"

#define STATE_MODULE    0
#define STATE_NAME      1
#define STATE_PARAMS    2
#define STATE_WAITING   3
#define STATE_CONTENTS  4
#define STATE_FINISHED  5

namespace Metabot
{
    Module::Module()
    {
    }

    Module::Module(std::string file_)
        : file(file_), cache(NULL)
    {
        state = STATE_MODULE;
        equals = 0;
        brackets = 1;
    }
            
    std::string Module::getFilename()
    {
        return file;
    }

    void Module::setCache(Cache *cache_)
    {
        cache = cache_;
    }

    void Module::setName(std::string name_)
    {
        name = name_;
    }

    std::string Module::getName()
    {
        return name;
    }

    void Module::setType(std::string type_)
    {
        type = type_;
    }

    std::string Module::getType()
    {
        return type;
    }

    void Module::setDescription(std::string description_)
    {
        description = description_;
    }

    std::string Module::getDescription()
    {
        return description;
    }
            
    Parameter &Module::getParameter(std::string name)
    {
        return parameters[name];
    }

    std::string Module::push(unsigned char c)
    {
        std::string output;

        switch (state) {
            case STATE_MODULE:
                if (c == ' ') {
                    state = STATE_NAME;
                }
                break;
            case STATE_NAME:
                // Parsing the name of the module
                if (c == '(') {
                    name = trim(name);
                    state = STATE_PARAMS;
                } else {
                    name += c;
                }
                break;
            case STATE_PARAMS:
                // Parsing parameters of the module
                if (c == ')') {
                    if (trim(tmpName) != "") {
                        parameters[trim(tmpName)].name = trim(tmpName);
                        parameters[trim(tmpName)].value = trim(tmpValue);
                    }
                    state = STATE_WAITING;
                } else if (c == ',') {
                    equals = 0;
                    if (trim(tmpName) != "") {
                        parameters[trim(tmpName)] = trim(tmpValue);
                        tmpName = "";
                        tmpValue = "";
                    }
                } else if (c == '=') {
                    equals = 1;
                } else {
                    if (equals) {
                        tmpValue += c;
                    } else {
                        tmpName += c;
                    }
                }
                break;
            case STATE_WAITING:
                // Waiting for the { to begin the module
                if (c == '{') {
                    state = STATE_CONTENTS;
                    std::ostringstream oss;
                    oss << "// metabot: Begining module " << name << std::endl;
                    if (type != "component") {
                        auto last = parameters.end();
                        auto iterator = parameters.begin();
                        last--;

                        // Adding type marker
                        oss << "marker(\"metabot_" << type << ": " << name << "\");" << std::endl;
                        // Adding parameters markers
                        for (auto entry : parameters) {
                            oss << "marker(\"metabot_parameter: " << entry.first << " \", " << entry.first << ");" << std::endl;
                        }
                        // Adding forwarding to the _ module
                        oss << "if (NoModels == false) {" << std::endl;
                        oss << "_" << name << "(";
                        for (iterator=parameters.begin(); iterator!=parameters.end(); iterator++) {
                            auto entry = *iterator;
                            oss << entry.first << "=" << entry.first;
                            if (iterator != last) {
                                oss << ",";
                            }
                        }
                        oss << ");" << std::endl;
                        oss << "}" << std::endl;
                        oss << "}" << std::endl;

                        // Creating forward module
                        oss << "module _" << name << "(";
                        for (iterator=parameters.begin(); iterator!=parameters.end(); iterator++) {
                            auto entry = *iterator;
                            oss << entry.first << "=" << entry.second.value;
                            if (iterator != last) {
                                oss << ", ";
                            }
                        }
                        oss << ") {" << std::endl;
                    }
                    output = oss.str();
                }
                break;
            case STATE_CONTENTS: {
                // Module contents, waiting it to finish, counting the brackets
                if (c == '{') {
                    brackets++;
                } 
                if (c == '}') {
                    brackets--;
                }
                if (brackets <= 0) {
                    std::ostringstream oss;
                    oss << "// metabot: Ending module " << name << std::endl;
                    output = oss.str();
                    state = STATE_FINISHED;
                }
                break;
                }
            case STATE_FINISHED:
                break;
        }

        return output;
    }
            
    std::string Module::pushLine(std::string line)
    {
        std::string output;
        for (unsigned int i = 0; i<line.length(); i++) {
            output += push(line[i]);
        }

        return output;
    }

    bool Module::finished()
    {
        return state == STATE_FINISHED;
    }
}
