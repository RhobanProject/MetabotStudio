#include <sstream>
#include "Values.h"

namespace Metabot
{
    bool Values::has(std::string name)
    {
        return count(name);
    }

    std::string Values::get(std::string name, std::string fallback)
    {
        return count(name) ? (*this)[name] : fallback;
    }

    void Values::set(std::string name, std::string value)
    {
        (*this)[name] = value;
    }

    Json::Value Values::toJson()
    {
        Json::Value json(Json::objectValue);

        for (auto entry : *this) {
            json[entry.first] = entry.second;
        }

        return json;
    }

    Values Values::fromJson(Json::Value json)
    {
        Values values;
        if (json.isObject()) {
            for (auto key : json.getMemberNames()) {
                if (json[key].isString()) {
                    values[key] = json[key].asString();
                } else if (json[key].isNumeric()) {
                    std::stringstream ss;
                    ss << json[key].asDouble();
                    values[key] = ss.str();
                }
            }
        }

        return values;
    }
}
