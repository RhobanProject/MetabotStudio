#include <iostream>
#include <sstream>
#include <fstream>
#include <json/json.h>
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
        std::ifstream f(filename);
        std::string line;

        while (std::getline(f, line)) {
            auto tmp = trim(line);
            line = replaceStr(tmp, ".scad", ".metabot.scad");

            switch (state) {
                case PARSER_NORMAL:
                case PARSER_TAGGED:
                    if (startswith(line, "//:")) {
                        auto parts = splitCSV(line.substr(3));
                        std::string annotation = strtolower(parts[0]);

                        if (annotation == "model" || annotation == "part" || annotation == "component") {
                            module.setType(annotation);
                            /*
                            if (parts.size() > 1) {
                                std::cout << "COLOR: " << parts[1] << std::endl;
                            }
                            */
                            // The module is tagged, we should analyze it
                            state = PARSER_TAGGED;
                        }
                        if (annotation == "description") {
                            module.setDescription(line.substr(12)); 
                        }
                        if (annotation == "parameter") {
                            if (parts.size() > 1) {
                                std::string name = parts[1];
                                std::string description = name;
                                if (parts.size() == 3) {
                                    description = parts[2];
                                }
                                module.getParameter(name).description = description;
                            }
                        }
                    } else {
                        oss << line << std::endl;

                        if (startswith(line, "module") && state == PARSER_TAGGED) {
                            // Entering in the module
                            oss << module.pushLine(line);
                            state = PARSER_MODULE;
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
                            json["male"] = (parts[2] == "male");
                            json["female"] = (parts[2] == "female");
                            oss << jsonMarker(json);
                        }
                        if (annotation == "bom" && parts.size()==4) {
                            Json::Value json;
                            json["type"] = "bom";
                            json["name"] = parts[1];
                            json["quantity"] = parts[2];
                            json["url"] = parts[3];
                            oss << jsonMarker(json);
                        }
                    } else {
                        oss << line << std::endl;
                        oss << module.pushLine(line);
                        if (module.finished()) {
                            state = PARSER_NORMAL;
                            modules.push_back(module);
                            module = Module(filename_out);
                        }
                    }
                break;
            }
        }

        // Writing the .metabot.scad file, if necessary
        if (filemtime(filename) > filemtime(filename_out)) {
            file_put_contents(filename_out, oss.str());
        }

        return modules;
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
