#include <sstream>
#include <json/json.h>
#include "TransformMatrix.h"
#ifdef OPENGL
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#endif

#define XY(x,y) ((x*4)+y)

namespace Metabot
{
    TransformMatrix::TransformMatrix()
    {
        values[XY(3,3)] = 1;
    }

    TransformMatrix TransformMatrix::zero()
    {
        TransformMatrix matrix;

        // Initializing to identity
        for (int x=0; x<4; x++) {
            for (int y=0; y<4; y++) {
                matrix.values[XY(x,y)] = 0;
            }
        }
    
        matrix.values[XY(3,3)] = 1;
        return matrix;
    }
            
    TransformMatrix TransformMatrix::invert()
    {
        TransformMatrix inverse;
        float *m = values;
        float *inv = inverse.values;
        double det;
        int i;

        inv[0] = m[5]  * m[10] * m[15] - 
                 m[5]  * m[11] * m[14] - 
                 m[9]  * m[6]  * m[15] + 
                 m[9]  * m[7]  * m[14] +
                 m[13] * m[6]  * m[11] - 
                 m[13] * m[7]  * m[10];

        inv[4] = -m[4]  * m[10] * m[15] + 
                  m[4]  * m[11] * m[14] + 
                  m[8]  * m[6]  * m[15] - 
                  m[8]  * m[7]  * m[14] - 
                  m[12] * m[6]  * m[11] + 
                  m[12] * m[7]  * m[10];

        inv[8] = m[4]  * m[9] * m[15] - 
                 m[4]  * m[11] * m[13] - 
                 m[8]  * m[5] * m[15] + 
                 m[8]  * m[7] * m[13] + 
                 m[12] * m[5] * m[11] - 
                 m[12] * m[7] * m[9];

        inv[12] = -m[4]  * m[9] * m[14] + 
                   m[4]  * m[10] * m[13] +
                   m[8]  * m[5] * m[14] - 
                   m[8]  * m[6] * m[13] - 
                   m[12] * m[5] * m[10] + 
                   m[12] * m[6] * m[9];

        inv[1] = -m[1]  * m[10] * m[15] + 
                  m[1]  * m[11] * m[14] + 
                  m[9]  * m[2] * m[15] - 
                  m[9]  * m[3] * m[14] - 
                  m[13] * m[2] * m[11] + 
                  m[13] * m[3] * m[10];

        inv[5] = m[0]  * m[10] * m[15] - 
                 m[0]  * m[11] * m[14] - 
                 m[8]  * m[2] * m[15] + 
                 m[8]  * m[3] * m[14] + 
                 m[12] * m[2] * m[11] - 
                 m[12] * m[3] * m[10];

        inv[9] = -m[0]  * m[9] * m[15] + 
                  m[0]  * m[11] * m[13] + 
                  m[8]  * m[1] * m[15] - 
                  m[8]  * m[3] * m[13] - 
                  m[12] * m[1] * m[11] + 
                  m[12] * m[3] * m[9];

        inv[13] = m[0]  * m[9] * m[14] - 
                  m[0]  * m[10] * m[13] - 
                  m[8]  * m[1] * m[14] + 
                  m[8]  * m[2] * m[13] + 
                  m[12] * m[1] * m[10] - 
                  m[12] * m[2] * m[9];

        inv[2] = m[1]  * m[6] * m[15] - 
                 m[1]  * m[7] * m[14] - 
                 m[5]  * m[2] * m[15] + 
                 m[5]  * m[3] * m[14] + 
                 m[13] * m[2] * m[7] - 
                 m[13] * m[3] * m[6];

        inv[6] = -m[0]  * m[6] * m[15] + 
                  m[0]  * m[7] * m[14] + 
                  m[4]  * m[2] * m[15] - 
                  m[4]  * m[3] * m[14] - 
                  m[12] * m[2] * m[7] + 
                  m[12] * m[3] * m[6];

        inv[10] = m[0]  * m[5] * m[15] - 
                  m[0]  * m[7] * m[13] - 
                  m[4]  * m[1] * m[15] + 
                  m[4]  * m[3] * m[13] + 
                  m[12] * m[1] * m[7] - 
                  m[12] * m[3] * m[5];

        inv[14] = -m[0]  * m[5] * m[14] + 
                   m[0]  * m[6] * m[13] + 
                   m[4]  * m[1] * m[14] - 
                   m[4]  * m[2] * m[13] - 
                   m[12] * m[1] * m[6] + 
                   m[12] * m[2] * m[5];

        inv[3] = -m[1] * m[6] * m[11] + 
                  m[1] * m[7] * m[10] + 
                  m[5] * m[2] * m[11] - 
                  m[5] * m[3] * m[10] - 
                  m[9] * m[2] * m[7] + 
                  m[9] * m[3] * m[6];

        inv[7] = m[0] * m[6] * m[11] - 
                 m[0] * m[7] * m[10] - 
                 m[4] * m[2] * m[11] + 
                 m[4] * m[3] * m[10] + 
                 m[8] * m[2] * m[7] - 
                 m[8] * m[3] * m[6];

        inv[11] = -m[0] * m[5] * m[11] + 
                   m[0] * m[7] * m[9] + 
                   m[4] * m[1] * m[11] - 
                   m[4] * m[3] * m[9] - 
                   m[8] * m[1] * m[7] + 
                   m[8] * m[3] * m[5];

        inv[15] = m[0] * m[5] * m[10] - 
                  m[0] * m[6] * m[9] - 
                  m[4] * m[1] * m[10] + 
                  m[4] * m[2] * m[9] + 
                  m[8] * m[1] * m[6] - 
                  m[8] * m[2] * m[5];

        det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
        det = 1.0 / det;

        for (i = 0; i < 16; i++) {
            inv[i] *= det;
        }

        return inverse;
    }

#ifdef OPENGL
    void TransformMatrix::openGLMult()
    {
        int i = 0;
        for (int x=0; x<4; x++) {
            for (int y=0; y<4; y++) {
                glmatrix[i++] = values[XY(y,x)];
            }
        }

        glMultMatrixf(glmatrix);
    }
#endif

    TransformMatrix TransformMatrix::identity()
    {
        TransformMatrix matrix;

        // Initializing to identity
        for (int x=0; x<4; x++) {
            for (int y=0; y<4; y++) {
                matrix.values[XY(x,y)] = (x==y);
            }
        }
        
        matrix.values[XY(3,3)] = 1;
        return matrix;
    }
            
    TransformMatrix TransformMatrix::fromJSON(std::string json)
    {
        TransformMatrix matrix;
        Json::Value root;
        Json::Reader reader;

        if (!reader.parse(json, root)) {
            throw std::string("Unable to decode matrix from data");
        }

        if (!root.isArray() || root.size()!=4) {
            throw std::string("Bad value for matrix");
        }

        for (int x=0; x<4; x++) {
            if (root[x].isArray() && root[x].size()==4) {
                for (int y=0; y<4; y++) {
                    if (root[x][y].isNumeric()) {
                        matrix.values[XY(x,y)] = root[x][y].asFloat();
                    } else {
                        throw std::string("Bad value for matrix");
                    }
                }
            } else {
                throw std::string("Bad dimension");
            }
        }

        return matrix;
    }

    TransformMatrix TransformMatrix::multiply(TransformMatrix &other)
    {
        TransformMatrix result;

        for (int x=0; x<4; x++) {
            for (int y=0; y<4; y++) {
                float value = 0.0;
                for (int i=0; i<4; i++) {
                    value += values[XY(x,i)] * other.values[XY(i,y)];
                }
                result.values[XY(x,y)] = value;
            }
        }

        return result;
    }
            
    Vector TransformMatrix::apply(Vector vector)
    {
        Vector output;

        for (int i=0; i<4; i++) {
            float value = 0;
            for (int y=0; y<4; y++) {
                value += vector.values[y] * values[XY(i,y)];
            }
            output.values[i] = value;
        }

        return output;
    }

    std::string TransformMatrix::toString()
    {
        std::stringstream str;

        for (int x=0; x<4; x++) {
            for (int y=0; y<4; y++) {
                str << values[XY(x,y)] << " ";
            }
            str << std::endl;
        }

        return str.str();
    }
}
