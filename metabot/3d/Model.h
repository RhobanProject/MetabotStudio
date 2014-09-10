#ifndef MODEL_H
#define MODEL_H

#include "Volume.h"
#include "Point3.h"
#include "../TransformMatrix.h"
using std::vector;

namespace Metabot
{
    class Model
    {
        public:
			vector<Volume> volumes;
			Model();
			virtual ~Model();
			Point3 min();
			Point3 max();
			bool contains(float x, float y);
			void merge(const Model &other);
            void apply(TransformMatrix matrix);
    		void translate(float X=0, float Y=0, float Z=0);
			void rotateZ(float r);
			void rotateY(float r);
			void rotateX(float r);
#ifdef OPENGL
            void openGLDraw();
#endif
            float r, g, b;
    };
}
#endif
