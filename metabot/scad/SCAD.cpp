#include <iostream>
#include <sstream>
#include <fstream>
#include <json/json.h>
#include "util.h"
#include "SCAD.h"

/**
 * Parser states
 */
#define PARSER_NORMAL    0 // Normal state
#define PARSER_TAGGED    1 // Header is tagged
#define PARSER_MODULE    2 // We are in a module

/**
 * Module states
 */
#define MODULE_MODULE    0 // The "module" keyword
#define MODULE_NAME      1 // Parsing the name of the module
#define MODULE_PARAMS    2 // Module parameters
#define MODULE_WAITING   3 // Waiting for the module starting {
#define MODULE_CONTENTS  4 // Module contents
#define MODULE_FINISHED  5 // Module is finished

namespace Metabot
{
    SCAD::SCAD(std::string filename_)
        : filename(filename_)
    {
    }
            
    std::vector<Module> SCAD::getModules()
    {
        return modules;
    }

    void SCAD::process()
    {
        // Parser state
        state = PARSER_NORMAL;
        // Module state
        moduleState = MODULE_MODULE;
        brackets = 1;
        equals = 0;

        oss.clear();
        outputFilename = filename.substr(0, filename.length()-5) + ".metabot.scad";
        oss << "// This file was automatically generated by Metabot" << std::endl;
        oss << "// DO NOT EDIT" << std::endl;
        oss << "NoModels = false;" << std::endl;
        
        // Modules found
        modules.clear();
        std::vector<Module> modules;
        // Current module
        module.setFilename(outputFilename);

        // Opening the file
        std::ifstream f(filename);
        std::string line;

        while (std::getline(f, line)) {
            auto tmp = trim(line);
            line = replaceStr(tmp, ".scad", ".metabot.scad");

            processLine(line);
        }

        // Writing the .metabot.scad file, if necessary
        if (filemtime(filename) > filemtime(outputFilename)) {
            file_put_contents(outputFilename, oss.str());
        }
    }
            
    void SCAD::processLine(std::string line)
    {
        switch (state) {
            case PARSER_NORMAL:
            case PARSER_TAGGED:
                if (startswith(line, "//:")) {
                    auto parts = splitCSV(line.substr(3));
                    std::string annotation = strtolower(parts[0]);

                    if (annotation == "model" || annotation == "part" || annotation == "component") {
                        module.setType(annotation);
                        if (parts.size() > 1) {
                            module.setPrettyName(parts[1]);
                        }
                        if (parts.size() > 2) {
                            module.setColor(parts[2]);
                        }
                        // The module is tagged, we should analyze it
                        state = PARSER_TAGGED;
                    }
                    if (annotation == "description" && parts.size()>=2) {
                        module.setDescription(parts[1]);
                    }
                    if (annotation == "parameter") {
                        if (parts.size() > 1) {
                            std::string name = parts[1];
                            std::string description = name;
                            if (parts.size() == 3) {
                                description = parts[2];
                            }
                            module.getParameter(name).description = description;
                            module.getParameter(name).expose = true;
                        }
                    }
                    if (annotation == "mass" && parts.size()>1) {
                        module.mass = atof(parts[1].c_str());
                    }
                    if (annotation == "density" && parts.size()>1) {
                        module.density = atof(parts[1].c_str());
                    }
                    if (annotation == "bom" && parts.size()==4) {
                        Json::Value json;
                        json["type"] = "bom";
                        json["name"] = parts[1];
                        json["price"] = atof(parts[2].c_str());
                        json["url"] = parts[3];
                        json["quantity"] = 1;
                        oss << jsonMarker(json);
                    }
                } else {
                    oss << line << std::endl;

                    if (startswith(line, "module") && state == PARSER_TAGGED) {
                        // Entering in the module
                        moduleState = MODULE_MODULE;
                        state = PARSER_MODULE;
                        processModuleLine(line);
                        brackets = 1;
                    }
                }
            break;
            case PARSER_MODULE:
                // Analyzing the module
                if (startswith(line, "//:")) {
                    auto parts = splitCSV(line.substr(3));
                    std::string annotation = strtolower(parts[0]);
                    if (annotation == "anchor" && parts.size()==3) {
                        Json::Value json;
                        json["type"] = "anchor";
                        json["name"] = parts[1];
                        json["male"] = (parts[2] == "male");
                        json["female"] = (parts[2] == "female");
                        oss << jsonMarker(json);
                    }
                    if (annotation == "bom" && parts.size()==4) {
                        Json::Value json;
                        json["type"] = "bom";
                        json["name"] = parts[1];
                        json["price"] = atof(parts[2].c_str());
                        json["url"] = parts[3];
                        json["quantity"] = 1;
                        oss << jsonMarker(json);
                    }
                    if (annotation == "tip") {
                        Json::Value json;
                        json["type"] = "tip";
                        oss << jsonMarker(json);
                    }
                } else {
                    oss << line << std::endl;
                    processModuleLine(line);
                    if (moduleState >= MODULE_FINISHED) {
                        state = PARSER_NORMAL;
                        modules.push_back(module);
                        module = Module();
                        module.setFilename(outputFilename);
                    }
                }
            break;
        }
    }

    void SCAD::processModuleLine(std::string line)
    {
        for (unsigned int i=0; i<line.size(); i++) {
            char c = line[i];
            switch (moduleState) {
                case MODULE_MODULE:
                    // Waiting the "module" keyword to be finished
                    if (c == ' ') {
                        tmpName = "";
                        moduleState = MODULE_NAME;
                    }
                    break;
                case MODULE_NAME:
                    // Parsing the name of the module
                    if (c == '(') {
                        module.setName(trim(tmpName));
                        tmpName = "";
                        moduleState = MODULE_PARAMS;
                    } else {
                        tmpName += c;
                    }
                    break;
                case MODULE_PARAMS:
                    // Parsing parameters of the module
                    if (c == ')') {
                        if (trim(tmpName) != "") {
                            module.getParameter(trim(tmpName)).value = trim(tmpValue);
                        }
                        moduleState = MODULE_WAITING;
                    } else if (c == ',') {
                        equals = 0;
                        if (trim(tmpName) != "") {
                            module.getParameter(trim(tmpName)).value = trim(tmpValue);
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
                case MODULE_WAITING:
                    // Waiting for the { to begin the module
                    if (c == '{') {
                        moduleState = MODULE_CONTENTS;
                        oss << "// metabot: Begining module " << module.getName() << std::endl;
                        if (module.getType() != "component") {
                            auto allParameters = module.getParameters();
                            auto last = allParameters.end();
                            auto iterator = allParameters.begin();
                            if (last != allParameters.begin()) {
                                last--;
                            }

                            // Adding type marker
                            oss << "marker(\"metabot: {";
                            oss << "'type': '" << module.getType() << "',";
                            oss << "'name': '" << module.getName() << "',";
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
                            oss << "}";
                            oss << ",'color': [" << module.r << "," << module.g << "," << module.b << "]";
                            if (module.mass > 0.0001) {
                                oss << ",'mass':" << module.mass;
                            }
                            if (module.density > 0.0001) {
                                oss << ",'density':" << module.density;
                            }
                            oss << "}\");" << std::endl;

                            // Adding forwarding to the _ module
                            oss << "if (NoModels == false) {" << std::endl;
                            oss << "_" << module.getName() << "(";
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
                            oss << "module _" << module.getName() << "(";
                            for (iterator=allParameters.begin(); iterator!=allParameters.end(); iterator++) {
                                auto entry = *iterator;
                                oss << entry.first << "=" << entry.second.value;
                                if (iterator != last) {
                                    oss << ",";
                                }
                            }
                            oss << ") {" << std::endl;
                        }
                    }
                    break;
                case MODULE_CONTENTS: {
                    // Module contents, waiting it to finish, counting the brackets
                    if (c == '{') {
                        brackets++;
                    } 
                    if (c == '}') {
                        brackets--;
                    }
                    if (brackets <= 0) {
                        oss << "// metabot: Ending module " << module.getName() << std::endl;
                        moduleState = MODULE_FINISHED;
                    }
                    break;
                    }
                case MODULE_FINISHED:
                    break;
            }
        }
    }

    std::vector<Module> SCAD::load(std::string filename)
    {
        SCAD scad(filename);
        scad.process();

        return scad.getModules();
    }

    std::string SCAD::jsonMarker(Json::Value json)
    {
        Json::FastWriter writer;
        std::string value = writer.write(json);

        for (unsigned int i=0; i<value.size(); i++) {
            if (value[i] == '"') {
                value[i] = '\'';
            }
        }

        std::stringstream ss;
        ss << "marker(\"metabot: " << trim(value) << "\");" << std::endl;
        return ss.str();
    }
}
