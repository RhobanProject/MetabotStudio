#include <iostream>
#include <sstream>
#include "Parts.h"
#include "util.h"

namespace Metabot
{
    Part::Part(std::string name_, std::string params_, TransformMatrix matrix_)
        : name(name_), params(params_), quantity(1), matrix(matrix_)
    {
        // std::cout << "Part: " << name << ", " << params << std::endl;
    }
            
    std::string Part::hash()
    {
        return hash_sha1(name+"//"+params);
    }

    void Parts::add(const Part &part)
    {
        parts.push_back(part);
    }
            
    void Parts::merge(const Parts &parts)
    {
        for (auto part : parts.parts) {
            add(part);
        }
    }
            
    PartsGrouped Parts::group()
    {
        std::map<std::string, std::string> hashToName;
        std::map<std::string, int> counts;
        PartsGrouped grouped;

        for (auto part: parts) {
            auto hash = part.hash();
            auto name = part.name;

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

            grouped.add(hashToName[hash], part);
        }

        return grouped;
    }
            
    void PartsGrouped::add(std::string name, Part part)
    {
        groups[name].add(part);
    }

    std::string PartsGrouped::toString()
    {
        std::ostringstream oss;

        int total = 0;
        int unique = 0;
        for (auto entry : groups) {
            auto name = entry.first;
            auto parts = entry.second;
            auto count = parts.parts.size();
            oss << "* " << name << ".stl " << count << std::endl;
            total += count;
            unique++;
        }
        oss << total << " parts to create (" << unique << " unique)" << std::endl;

        return oss.str();
    }
}
