#pragma once

#include <string>
#include <map>
#include "Parameter.h"
#include <json/json.h>

namespace Metabot
{
    class Parameters : public std::map<std::string, Parameter>
    {
        public:
            Parameters();

            void set(std::string name, std::string value);
            void set(std::string name, double value);
            void set(std::string name, Parameter parameter);
            void update(std::string name, std::string value);
            Parameter &getParameter(std::string name);
            std::string get(std::string name, std::string fallback="");
            bool has(std::string name);
            void merge(const Parameters &other);
            
            Json::Value toJson();
            static Parameters fromJson(Json::Value);

            std::string toArgs();
    };
}
