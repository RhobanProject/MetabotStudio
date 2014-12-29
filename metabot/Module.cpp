#include <sstream>
#include <iostream>
#include "Cache.h"
#include "Module.h"
#include "SCAD.h"
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

    Module::Module(std::string filename_)
        : filename(filename_), backend(NULL)
    {
        state = STATE_MODULE;
        equals = 0;
        brackets = 1;
    }
            
    std::string Module::getFilename()
    {
        return filename;
    }

    void Module::setBackend(Backend *backend_)
    {
        backend = backend_;
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
        return parameters.get(name);
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
                        parameters.get(trim(tmpName)).value = trim(tmpValue);
                    }
                    state = STATE_WAITING;
                } else if (c == ',') {
                    equals = 0;
                    if (trim(tmpName) != "") {
                        parameters.get(trim(tmpName)).value = trim(tmpValue);
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
                        auto allParameters = parameters.getAll();
                        auto last = allParameters.end();
                        auto iterator = allParameters.begin();
                        if (last != allParameters.begin()) {
                            last--;
                        }

                        // Adding type marker
                        oss << "marker(\"metabot: {";
                        oss << "'type': '" << type << "',";
                        oss << "'name': '" << name << "',";
                        oss << "'parameters': {";
                        for (iterator=allParameters.begin(); iterator!=allParameters.end(); iterator++) {
                            auto entry = *iterator;
                            oss << "'" << entry.first << "':";
                            if (entry.second.isString()) {
                                oss << "'";
                            }
                            oss << "\",";
                            oss << entry.first << ",\"";
                            if (entry.second.isString()) {
                                oss << "'";
                            }
                            if (iterator != last) {
                                oss << ",";
                            }
                        }
                        oss << "}}\");" << std::endl;

                        // Adding forwarding to the _ module
                        oss << "if (NoModels == false) {" << std::endl;
                        oss << "_" << name << "(";
                        for (iterator=allParameters.begin(); iterator!=allParameters.end(); iterator++) {
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
                        for (iterator=allParameters.begin(); iterator!=allParameters.end(); iterator++) {
                            auto entry = *iterator;
                            oss << entry.first << "=" << entry.second.value;
                            if (iterator != last) {
                                oss << ",";
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
  
    std::string Module::openscad(std::string format, Parameters parameters, bool noModels)
    {
        std::string key = hash_sha1(filename + "." + format + " [" + (noModels ? "y" : "n") + "] w/ " + parameters.toArgs());
        if (backend->cache != NULL) {
            return backend->cache->get(key, [this, format, parameters, noModels]() {
                return this->doOpenscad(format, parameters, noModels);
            }, filename);
        } else {
            return doOpenscad(format, parameters, noModels);
        }
    }
    
    std::string Module::doOpenscad(std::string format, Parameters parameters, bool noModels)
    {
        std::stringstream cmd;
        cmd << "openscad ";
        std::string input = tempname() + ".scad";
        std::string output = tempname() + "." + format;
        if (noModels) {
            cmd << "-DNoModels=true ";
        }
        cmd << input << " -o " << output;
        // cmd << " >/dev/null 2>/dev/null";
        std::string command = cmd.str();

        std::stringstream scad;
        scad << "use <" << current_dir() << "/" << filename << ">;" << std::endl;
        scad << std::endl;
        scad << name << "(";
        auto allParameters = parameters.getAll();
        auto last = allParameters.end();
        if (last != allParameters.begin()) {
            last--;
        }
        auto iterator = allParameters.begin();
        for (iterator=allParameters.begin(); iterator!=allParameters.end(); iterator++) {
            auto entry = *iterator;
            scad << entry.first << "=" << entry.second.value;
            if (iterator != last) {
                scad << ", ";
            }
        }
        scad << ");" << std::endl;
        file_put_contents(input, scad.str());
        
        // Uncomment that to see the compile command called
        std::cout << "compile(): " << command << std::endl;

        FILE *process = popen(command.c_str(), "r");
        int result = pclose(process);
        remove(input.c_str());
        if (result != 0) {
            return "";
            /*
            std::stringstream error;
            error << "Compilation failed for file " << filename;
            throw error.str();
            */
        }

        if (!file_exists(output)) {
            std::stringstream error;
            error << "Compilation did not produced the output file";
            throw error.str();
        }

        std::string data = file_get_contents(output);
        remove(output.c_str());

        return data;
    }
}
