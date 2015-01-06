#include <math.h>
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

namespace Metabot
{
    TransformMatrix::TransformMatrix()
    {
        values[3][3] = 1;
    }

    TransformMatrix TransformMatrix::zero()
    {
        TransformMatrix matrix;

        VEC_ZERO_4(matrix.values[0]);
        VEC_ZERO_4(matrix.values[1]);
        VEC_ZERO_4(matrix.values[2]);
        VEC_ZERO_4(matrix.values[3]);
    
        matrix.values[3][3] = 1;
        return matrix;
    }
            
    TransformMatrix TransformMatrix::invert()
    {
        TransformMatrix inverse;
        float det;
        INVERT_4X4(inverse.values, det, values);

        return inverse;
    }

#ifdef OPENGL
    void TransformMatrix::openGLMult()
    {
        int i = 0;
        for (int x=0; x<4; x++) {
            for (int y=0; y<4; y++) {
                glmatrix[i++] = values[y][x];
            }
        }

        glMultMatrixf(glmatrix);
    }
#endif

    TransformMatrix TransformMatrix::identity()
    {
        TransformMatrix matrix;
        IDENTIFY_MATRIX_4X4(matrix.values);

        return matrix;
    }
            
    TransformMatrix TransformMatrix::fromJSON(std::string json)
    {
        TransformMatrix matrix;
        Json::Value root;
        Json::Reader reader;

        if (!reader.parse(json, root)) {
            std::stringstream ss;
            ss << "Unable to decode matrix from: " << json;
            throw ss.str();
        }

        if (!root.isArray() || root.size()!=4) {
            throw std::string("Bad value for matrix");
        }

        for (int x=0; x<4; x++) {
            if (root[x].isArray() && root[x].size()==4) {
                for (int y=0; y<4; y++) {
                    if (root[x][y].isNumeric()) {
                        matrix.values[x][y] = root[x][y].asFloat();
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

    TransformMatrix TransformMatrix::multiply(TransformMatrix other)
    {
        TransformMatrix result;
        MATRIX_PRODUCT_4X4(result.values, values, other.values);

        return result;
    }
            
    Vector TransformMatrix::apply(Vector vector)
    {
        Vector output;
        MAT_DOT_VEC_4X4(output.values, values, vector.values);

        return output;
    }

    std::string TransformMatrix::toString()
    {
        std::stringstream str;

        for (int x=0; x<4; x++) {
            for (int y=0; y<4; y++) {
                str << values[x][y] << " ";
            }
            str << std::endl;
        }

        return str.str();
    }

    TransformMatrix TransformMatrix::rotationX(float alpha)
    {
        TransformMatrix m = TransformMatrix::identity();
        m.values[1][1] = cos(alpha);
        m.values[2][1] = -sin(alpha);
        m.values[1][2] = sin(alpha);
        m.values[2][2] = cos(alpha);
        return m;
    }
    TransformMatrix TransformMatrix::rotationY(float alpha)
    {
        TransformMatrix m = TransformMatrix::identity();
        m.values[0][0] = cos(alpha);
        m.values[2][0] = -sin(alpha);
        m.values[0][2] = sin(alpha);
        m.values[2][2] = cos(alpha);
        return m;
    }
    TransformMatrix TransformMatrix::rotationZ(float alpha)
    {
        TransformMatrix m = TransformMatrix::identity();
        m.values[0][0] = cos(alpha);
        m.values[1][0] = -sin(alpha);
        m.values[0][1] = sin(alpha);
        m.values[1][1] = cos(alpha);
        return m;
    }
}
