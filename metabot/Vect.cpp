#include <sstream>
#include <cmath>
#include <btBulletDynamicsCommon.h>
#include "Vect.h"

namespace Metabot
{
    Vect::Vect()
    {
        values[3] = 1;
    }

    Vect::Vect(float x, float y, float z)
    {
        values[0] = x;
        values[1] = y;
        values[2] = z;
        values[3] = 1;
    }

    std::string Vect::toString() const
    {
        std::stringstream str;

        for (int i=0; i<4; i++) {
            str << values[i] << std::endl;
        }

        return str.str();
    }
            
    Json::Value Vect::toJson()
    {
        Json::Value json(Json::arrayValue);
        json[0] = values[0];
        json[1] = values[1];
        json[2] = values[2];

        return json;
    }

    Vect Vect::fromJson(Json::Value json)
    {
        if (json.isArray() && json.size()==3) {
            return Vect(json[0].asFloat(), json[1].asFloat(), json[2].asFloat());
        } else {
            return Vect(0, 0, 0);
        }
    }
            
    Vect Vect::add(const Vect &other)
    {
        return Vect(values[0]+other.values[0], values[1]+other.values[1],
                values[2]+other.values[2]);
    }
            
    Vect Vect::multiply(double f)
    {
        return Vect(values[0]*f, values[1]*f, values[2]*f);
    }
            
    float Vect::norm()
    {
        return distance(Vect(0, 0, 0));
    }
            
    float Vect::distance(const Vect &other)
    {
        float s = 0;
        for (int i=0; i<3; i++) {
            s += pow(other.values[i]-values[i],2);
        }

        return sqrt(s);
    }

    float Vect::x() const
    {
        return values[0];
    }

    float Vect::y() const
    {
        return values[1];
    }

    float Vect::z() const
    {
        return values[2];
    }
            
    double Vect::dot(Vect other)
    {
        return values[0]*other.values[0] +
            values[1]*other.values[1] +
            values[2]*other.values[2]
            ;
    }

    Vect Vect::fromBullet(btVector3 bvect)
    {
        return Vect(bvect.x(), bvect.y(), bvect.z());
    }
            
    btVector3 Vect::toBullet()
    {
        return btVector3(x(), y(), z());
    }
}
