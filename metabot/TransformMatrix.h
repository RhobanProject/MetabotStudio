#pragma once

#include <limits>
#include <array>
#include <string>
#include "Vector.h"
#ifdef OPENGL
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#endif
class Symbolic;
#include "vvector.h"

std::array<float,3> eulerAngles(const double4x4& R);

namespace Metabot
{
    class TransformMatrix
    {
        public:
            TransformMatrix();
            TransformMatrix multiply(TransformMatrix other);
            TransformMatrix invert();
            std::string toString();

            std::string toJS();
            std::string toURDF();

            Vector apply(Vector vector);

            double4x4 values;

            Symbolic toSymbolic();

#ifdef OPENGL
            void openGLMult();
            GLfloat glmatrix[4*4];
#endif
            
            static TransformMatrix identity();
            static TransformMatrix zero();
            static TransformMatrix rotationX(float alpha);
            static TransformMatrix rotationY(float alpha);
            static TransformMatrix rotationZ(float alpha);
            static TransformMatrix translation(float x, float y, float z);
            static TransformMatrix fromJSON(std::string json);

            float x();
            float y();
            float z();
    };
}
