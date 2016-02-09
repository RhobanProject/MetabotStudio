#include <sstream>
#include "Parameters.h"

namespace Metabot
{
    Parameters::Parameters()
    {
    }

    void Parameters::set(std::string name, std::string value)
    {
        getParameter(name).value = value;
    }
            
    void Parameters::set(std::string name, double value)
    {
        std::stringstream ss;
        ss << value;
        set(name, ss.str());
    }
            
    void Parameters::set(std::string name, Parameter parameter)
    {
        (*this)[name] = parameter;
    }
    
    void Parameters::update(std::string name, std::string value)
    {
        getParameter(name).updateValue(value);
    }
            
    bool Parameters::has(std::string name)
    {
        return count(name);
    }
            
    void Parameters::merge(const Parameters &other)
    {
        for (auto entry : other) {
            set(entry.first, entry.second);
        }
    }
    
    Parameter &Parameters::getParameter(std::string name)
    {
        if (!count(name)) {
            (*this)[name].name = name;
        }

        return (*this)[name];
    }

    std::string Parameters::get(std::string name, std::string fallback)
    {
        if (!count(name)) {
            return fallback;
        }

        return (*this)[name].value;
    }
            
    std::string Parameters::toArgs()
    {
        std::string args = "";
        
        for (auto parameter : *this) {
            args += "-D" + parameter.second.name + "=" + parameter.second.value + " ";
        }

        return args;
    }
    
    Json::Value Parameters::toJson()
    {
        Json::Value json(Json::objectValue);

        for (auto entry : *this) {
            json[entry.first] = Json::Value(Json::objectValue);
            json[entry.first]["val"] = entry.second.value;
            json[entry.first]["min"] = entry.second.min;
            json[entry.first]["max"] = entry.second.max;
        }

        return json;
    }

    Parameters Parameters::fromJson(Json::Value json)
    {
        Parameters parameters;
        if (json.isObject()) {
            for (auto key : json.getMemberNames()) {
                Parameter p;
                p.name = key;
                if (json[key].isObject()) {
                    if (json[key].isMember("val")) p.value = json[key]["val"].asString();
                    if (json[key].isMember("min")) p.min = json[key]["min"].asString();
                    if (json[key].isMember("max")) p.max = json[key]["max"].asString();
                } else if (json[key].isString()) {
                    p.value = json[key].asString();
                } else if (json[key].isNumeric()) {
                    std::stringstream ss;
                    ss << json[key].asFloat();
                    p.value = ss.str();
                }
                parameters[key] = p;
            }
        }

        return parameters;
    }
}
