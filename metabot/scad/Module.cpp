#include <sstream>
#include <iostream>
#include "Cache.h"
#include "Module.h"
#include "SCAD.h"
#include "util.h"

namespace Metabot
{
    Module::Module()
        : backend(NULL)
    {
        prettyName = "";
        description = "";
        r = 0.8;
        g = 0.8;
        b = 0.8;
        mass = 0.0;
        density = 1.25;
    }

    Module::~Module()
    {
    }

    void Module::setFilename(std::string filename_)
    {
        filename = filename_;
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
        if (prettyName == "") {
            prettyName = name;
        }
    }

    std::string Module::getName()
    {
        return name;
    }
            
    void Module::setPrettyName(std::string prettyName_)
    {
        prettyName = prettyName_;
    }

    std::string Module::getPrettyName()
    {
        return prettyName;
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
        description = trim(description_);
    }

    std::string Module::getDescription()
    {
        if (description != "") {
            return description;
        } else {
            return name;
        }
    }

    bool Module::hasParameter(std::string name)
    {
        return parameters.count(name);
    }

    Parameters &Module::getParameters()
    {
        return parameters;
    }
            
    Parameter &Module::getParameter(std::string name)
    {
        return parameters.get(name);
    }

    std::string Module::openscad(std::string format, Parameters parameters, int defines)
    {
        if (backend == NULL) {
            return "";
        }

        std::stringstream ss;
        ss << filename << "." << format << " [" << defines << "] " << parameters.toArgs();
        std::string key = hash_sha1(ss.str());

        if (backend->cache != NULL) {
            return backend->cache->get(key, [this, format, parameters, defines]() {
                return this->doOpenscad(format, parameters, defines);
            }, filename);
        } else {
            return doOpenscad(format, parameters, defines);
        }
    }
    
    std::string Module::doOpenscad(std::string format, Parameters parameters, int defines)
    {
        std::stringstream cmd;
        cmd << "openscad ";
        std::string input = tempname() + ".scad";
        std::string output = tempname() + "." + format;
        if (defines & DEFINE_NO_MODELS) {
            cmd << "-DNoModels=true ";
        }
        if (defines & DEFINE_COLLISIONS) {
            cmd << "-DCollisions=true ";
        }
        if (defines & DEFINE_JS) {
            cmd << "-DJS=true ";
        }
        if (!parameters.has("$fn")) {
            cmd << "-D\\$fn=20 ";
        }
        cmd << input << " -o " << output;
        // cmd << " >/dev/null 2>/dev/null";
        std::string command = cmd.str();

        std::stringstream scad;
        scad << "use <" << current_dir() << "/" << filename << ">;" << std::endl;
        scad << std::endl;
        scad << "// This is a temprary file generated by Metabot" << std::endl;
        scad << "// Rendering module " << getName() << " to format " << format << std::endl;
        scad << "// You can safely remove this file is Metabot is not running" << std::endl;
        scad << name << "(";
        auto allParameters = parameters;
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
            
    void Module::setColor(std::string color)
    {
        Json::Reader reader;
        Json::Value json;

        if (reader.parse(color, json)) {
            if (json.isArray() && json.size() == 3) {
                r = json[0].asFloat();
                g = json[1].asFloat();
                b = json[2].asFloat();
            }
        }
    }
}