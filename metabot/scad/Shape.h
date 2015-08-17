#pragma once

#include "TransformMatrix.h"
#include "Vect.h"

#define SHAPE_BOX       0
#define SHAPE_SPHERE    1
#define SHAPE_CYLINDER  2

/**
 * Pure shapes
 */
namespace Metabot
{
    class Shape
    {
        public:
            TransformMatrix matrix;
            std::string toURDF(TransformMatrix transform);

            int type;
            double a, b, c;
            double r, h;
    };
}
