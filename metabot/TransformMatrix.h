#pragma once

#include <string>
#include "Vector.h"
#ifdef OPENGL
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#endif

namespace Metabot
{
    class TransformMatrix
    {
        public:
            TransformMatrix();
            TransformMatrix multiply(TransformMatrix other);
            TransformMatrix invert();
            std::string toString();

            Vector apply(Vector vector);

            float values[4*4];

#ifdef OPENGL
            void openGLMult();
            GLfloat glmatrix[4*4];
#endif
            
            static TransformMatrix identity();
            static TransformMatrix zero();
            static TransformMatrix rotationX(float alpha);
            static TransformMatrix rotationY(float alpha);
            static TransformMatrix rotationZ(float alpha);
            static TransformMatrix fromJSON(std::string json);
    };
}
