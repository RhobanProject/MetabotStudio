#pragma once

#include <map>
#include <string>
#include <json/json.h>

namespace Metabot
{
    class Values : public std::map<std::string, std::string>
    {
        public:
            bool has(std::string name);
            std::string get(std::string name, std::string fallback="");
            void set(std::string name, std::string value);
            void set(std::string name, double value);

            Json::Value toJson();

            static Values fromJson(Json::Value);
    };
}
