#pragma once

#include <string>
#include "Vector.h"

namespace Metabot
{
    class TransformMatrix
    {
        public:
            TransformMatrix();
            TransformMatrix multiply(TransformMatrix &other);
            std::string toString();

            Vector apply(Vector vector);

            float values[4][4];
            
            static TransformMatrix identity();
            static TransformMatrix zero();
            static TransformMatrix fromJSON(std::string json);
    };
}
