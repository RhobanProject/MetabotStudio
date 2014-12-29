#include <iostream>
#include <sstream>
#include "Ref.h"
#include "util.h"

namespace Metabot
{
    Ref::Ref(Json::Value json, TransformMatrix matrix_)
        : quantity(1), matrix(matrix_)
    {
        if (json.isObject()) {
            if (json.isMember("name")) {
                // Reading name
                name = json["name"].asString();
            }
            if (json.isMember("color") && json["color"].isArray()
                    && json["color"].size() == 3) {
                r = json["color"][0].asFloat();
                g = json["color"][1].asFloat();
                b = json["color"][2].asFloat();
            }
            if (json.isMember("parameters") && json["parameters"].isObject()) {
                // Reading parameters
                for (auto key : json["parameters"].getMemberNames()) {
                    if (json["parameters"][key].isString()) {
                        std::stringstream ss;
                        ss << "\"" << json["parameters"][key].asString() << "\"";
                        parameters.set(key, ss.str());
                    } else {
                        parameters.set(key, json["parameters"][key].asString());
                    }
                }
            }
        }
    }
            
    std::string Ref::hash()
    {
        return hash_sha1(name+"//"+parameters.toArgs());
    }

    void Refs::add(const Ref &ref)
    {
        push_back(ref);
    }
            
    void Refs::merge(const Refs &refs)
    {
        for (auto ref : refs) {
            add(ref);
        }
    }
            
    RefsGrouped Refs::group()
    {
        std::map<std::string, std::string> hashToName;
        std::map<std::string, int> counts;
        RefsGrouped grouped;

        for (auto ref: *this) {
            auto hash = ref.hash();
            auto name = ref.name;

            if (!hashToName.count(hash)) {
                if (!counts.count(name)) {
                    hashToName[hash] = name;
                    counts[name] = 1;
                } else {
                    counts[name] = counts[name]+1;
                    std::ostringstream oss;
                    oss << name << "_" << counts[name];
                    hashToName[hash] = oss.str();
                }
            }

            grouped.add(hashToName[hash], ref);
        }

        return grouped;
    }
            
    void RefsGrouped::add(std::string name, Ref ref)
    {
        groups[name].add(ref);
    }

    std::string RefsGrouped::toString()
    {
        std::ostringstream oss;

        int total = 0;
        int unique = 0;
        for (auto entry : groups) {
            auto name = entry.first;
            auto refs = entry.second;
            auto count = refs.size();
            oss << "* " << name << ".stl " << count << std::endl;
            total += count;
            unique++;
        }
        oss << total << " parts to create (" << unique << " unique)" << std::endl;

        return oss.str();
    }
}
