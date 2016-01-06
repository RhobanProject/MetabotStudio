#pragma once

#include <string>
#include <json/json.h>

class btVector3;
namespace Metabot
{
    class Vect
    {
        public:
            Vect(float x, float y, float z);
            Vect();

            Vect add(const Vect &other);
            Vect multiply(double f);
            float norm();
            float distance(const Vect &other);
            float values[4];
            float x() const;
            float y() const;
            float z() const;

            std::string toString() const;

            Json::Value toJson();
            static Vect fromJson(Json::Value json);

            double dot(Vect other);
            static Vect fromBullet(btVector3 bvect);
    };
}
