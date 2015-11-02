#include <iostream>
#include <sstream>
#include <json/json.h>
#include "Dynamics.h"

namespace Metabot
{
    Dynamics::Dynamics()
        : com(0, 0, 0), volume(0), mass(0), computed(false),
          ixx(0), ixy(0), ixz(0), iyy(0), iyz(0), izz(0)
    {
    }
            
    void Dynamics::combine(const Dynamics &other, TransformMatrix matrix)
    {
        if (computed) {
            if (other.computed) {
                auto other_com = matrix.apply(other.com);
                double a = mass;
                double b = other.mass;
                double total = a+b;
                if (total > 0.0) {
                    com.values[0] = (com.x()*a + other_com.x()*b)/total;
                    com.values[1] = (com.y()*a + other_com.y()*b)/total;
                    com.values[2] = (com.z()*a + other_com.z()*b)/total;
                }
                volume = volume+other.volume;
                mass = total;
                computed = true;

            }
        } else {
            if (other.computed) {
                computed = true;
                com = matrix.apply(other.com);
                mass = other.mass;
                volume = other.volume;
            }
        }
        for (auto cube : other.cubes) {
            cube.pos = matrix.apply(cube.pos);
            cubes.push_back(cube);
        }
        updateInertia();
    }

    std::string Dynamics::toString()
    {
        std::stringstream ss;
        if (computed) {
            ss << "Volume: " << volume << "mm^3" << std::endl;
            ss << "Mass: " << mass << "g" << std::endl;
            ss << "COM: " << com.x() << ", " << com.y() << ", " << com.z() << std::endl;
            ss << "Inertia: ixx=" << ixx << ", iyy=" << iyy << ", izz=" << izz << 
                ", ixy=" << ixy << ", iyz=" << iyz << ", ixz=" << ixz << std::endl;
        } else {
            ss << "No dynamics (no geometry or not computed)" << std::endl;
        }

        return ss.str();
    }
            
    void Dynamics::updateInertia()
    {
        ixx = iyy = izz = ixy = iyz = ixz = 0;

        for (auto &cube : cubes) {
            float X = cube.pos.x()/1000.0;
            float Y = cube.pos.y()/1000.0;
            float Z = cube.pos.z()/1000.0;
            float m = cube.mass/1000.0;

            ixx += (Y*Y + Z*Z)*m;
            iyy += (X*X + Z*Z)*m;
            izz += (X*X + Y*Y)*m;
            ixy -= X*Y*m;
            iyz -= Y*Z*m;
            ixz -= X*Z*m;
        }
    }
            
    std::string Dynamics::serialize()
    {
        Json::Value json;
        json["computed"] = computed;
        json["volume"] = volume;
        json["mass"] = mass;
        json["com"] = Json::Value(Json::arrayValue);
        json["com"][0] = com.values[0];
        json["com"][1] = com.values[1];
        json["com"][2] = com.values[2];

        int k = 0;
        json["cubes"] = Json::Value(Json::arrayValue);
        for (auto cube : cubes) {
            json["cubes"][k] = Json::Value(Json::arrayValue);
            json["cubes"][k][0] = cube.pos.values[0];
            json["cubes"][k][1] = cube.pos.values[1];
            json["cubes"][k][2] = cube.pos.values[2];
            json["cubes"][k][3] = cube.mass;
            k++;
        }

        Json::FastWriter writer;
        return writer.write(json);
    }
    
    Dynamics Dynamics::unserialize(std::string data)
    {
        Dynamics dynamics;
        Json::Value json;
        Json::Reader reader;

        if (reader.parse(data, json)) {
            dynamics.computed = json["computed"].asBool();
            dynamics.volume = json["volume"].asFloat();
            dynamics.mass = json["mass"].asFloat();
            dynamics.com.values[0] = json["com"][0].asFloat();
            dynamics.com.values[1] = json["com"][1].asFloat();
            dynamics.com.values[2] = json["com"][2].asFloat();

            for (auto cube : json["cubes"]) {
                DynamicsCube c;
                c.pos.values[0] = cube[0].asFloat();
                c.pos.values[1] = cube[1].asFloat();
                c.pos.values[2] = cube[2].asFloat();
                c.mass = cube[3].asFloat();

                dynamics.cubes.push_back(c);
            }
        }
        dynamics.updateInertia();

        return dynamics;
    }
}
