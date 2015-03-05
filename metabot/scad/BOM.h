#ifndef _METABOT_BOM_H
#define _METABOT_BOM_H

#include <string>
#include <map>
#include <json/json.h>

namespace Metabot
{
    class BOMEntry
    {
        public:
            BOMEntry();
            BOMEntry(Json::Value json);

            std::string name;
            std::string url;
            int quantity;
            float price;
    };

    class BOM
    {
        public:
            BOM();
            void merge(const BOM &other);
            void append(const BOMEntry &entry);
            std::string toString();

            std::map<std::string, BOMEntry> entries;
    };
}

#endif
