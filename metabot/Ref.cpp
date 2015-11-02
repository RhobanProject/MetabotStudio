#include <iostream>
#include <sstream>
#include <3d/stl.h>
#include "util/sha1.h"
#include "Cache.h"
#include "Ref.h"
#include "Backend.h"
#include "Voxels.h"
#include "util.h"

namespace Metabot
{
    Ref::Ref(Json::Value json, TransformMatrix matrix_, int default_defines_)
        : matrix(matrix_), default_defines(default_defines_)
    {
        if (json.isObject()) {
            if (json.isMember("name")) {
                // Reading name
                name = json["name"].asString();
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

    void Ref::compile(Backend *backend, int defines)
    {
        model = doCompile(backend, defines);
    }

    Model Ref::doCompile(Backend *backend, int defines, bool print)
    {
        defines |= default_defines;
        auto module = backend->getModule(name);
        auto params = parameters;

        r = module.r;
        g = module.g;
        b = module.b;
        mass = module.mass;
        density = module.density;

        if (print && module.hasParameter("print")) {
            std::cout << "[Debug] Passing print to true for " << name << std::endl;
            params.set("print", "true");
        }

        std::string stl = module.openscad("stl", params, defines);
        return loadModelSTL_string(stl);
    }

    void Ref::analyze(Backend *backend)
    {
        auto module = backend->getModule(name);
        std::stringstream ss;
        ss << module.getFilename() << "#" << module.getName() << " dynamics";
        std::string key = hash_sha1(ss.str()) + ".dynamics";

        std::string data = backend->cache->get(key, [this]() {
            std::cout << "Analyzing part " << name << "... " << std::endl << std::flush;
            auto dynamics = Voxels::voxelize(this->getModel(), this->density, this->mass);
            std::cout << dynamics.toString();
            std::cout << std::endl;

            return dynamics.serialize();
        }, module.getFilename());

        dynamics = Dynamics::unserialize(data);
    }

    Model &Ref::getModel()
    {
        return model;
    }

    Dynamics &Ref::getDynamics()
    {
        return dynamics;
    }

    void Ref::setDynamics(Dynamics other)
    {
        dynamics = other;
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
        (*this)[name].push_back(ref);
    }

    std::string RefsGrouped::toString()
    {
        std::ostringstream oss;

        int total = 0;
        int unique = 0;
        for (auto entry : (*this)) {
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
