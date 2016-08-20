#include <sstream>
#include <iostream>
#include <json/json.h>
#include "BOM.h"
#include "util.h"

namespace Metabot
{
    BOMEntry::BOMEntry()
        : name(""), url(""), quantity(0), price(0)
    {
    }

    BOMEntry::BOMEntry(Json::Value json)
        : name(""), url(""), quantity(0), price(0)
    {
        if (json.isObject()) {
            if (json.isMember("name") && json["name"].isString()) {
                name = json["name"].asString();
            }
            if (json.isMember("url") && json["url"].isString()) {
                url = json["url"].asString();
            }
            if (json.isMember("quantity") && json["quantity"].isNumeric()) {
                quantity = json["quantity"].asInt();
            }
            if (json.isMember("price") && json["price"].isNumeric()) {
                price = json["price"].asFloat();
            }
        }
    }

    BOM::BOM()
    {
    }
        
    void BOM::append(const BOMEntry &entry)
    {
        if (entries.count(entry.name)) {
            entries[entry.name].quantity += entry.quantity;
        } else {
            entries[entry.name] = entry;
        }
    }
            
    void BOM::merge(const BOM &other)
    {
        for (auto entry : other.entries) {
            append(entry.second);
        }
    }

    std::string BOM::toString()
    {
        std::ostringstream oss;
        float price = 0.0;
        for (auto all : entries) {
            BOMEntry entry = all.second;
            if (entry.quantity) {
                oss << "* " << entry.name << ": " << entry.quantity;
                if (entry.price > 0.0) {
                    oss << " x " << entry.price << "$ = ";
                    oss << (entry.quantity*entry.price) << "$";
                    price += entry.quantity*entry.price;
                }
                if (entry.url != "") {
                    oss << " (" << entry.url << ")";
                }
                oss << std::endl;
            }
        }
        if (price > 0.0) {
            oss << "Total: " << price << "$" << std::endl;
        }
        return oss.str();
    }
    
    std::string BOM::toHTML()
    {
        std::ostringstream oss;
        float price = 0.0;
        oss << "<style>td { padding:3px; } table { margin-bottom: 5px; }</style>";
        oss << "<table>";
        oss << "<thead>";
        oss << "<tr>";
        oss << "<td><b>Name</b></td> <td><b>Quantity</b></td> <td><b>Price</b></td>"
            << "<td><b>Total</b></td> <td><b>More</b></td>";
        oss << "</tr>";
        oss << "</thead>";
        for (auto all : entries) {
            oss << "<tr>";
            BOMEntry entry = all.second;
            if (entry.quantity) {
                oss << "<td><b>" << entry.name << "</b></td>";
                oss << "<td>" << entry.quantity << "</td>";
                oss << "<td>$ " << entry.price << "</td>";
                oss << "<td>$ " << entry.quantity*entry.price << "</td>";
                price += entry.quantity*entry.price;

                if (entry.url != "") {
                    oss << "<td><a href=\"" << entry.url << "\">Buy</a></td>";
                } else {
                    oss << "<td>-</td>";
                }
                oss << std::endl;
            }
            oss << "</tr>";
        }
        oss << "<tr><td></td><td></td><td></td><td><b>$ " << price << "</b></td><td></td></tr>";
        oss << "</table>";
        return oss.str();
    }
}
