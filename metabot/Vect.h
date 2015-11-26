#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include <string>

namespace Metabot
{
    class Vect
    {
        public:
            Vect(float x, float y, float z);
            Vect();

            float distance(const Vect &other);
            float values[4];
            float x() const;
            float y() const;
            float z() const;

            std::string toString() const;

            double dot(Vect other);
            static Vect fromBullet(btVector3 bvect);
    };
}
