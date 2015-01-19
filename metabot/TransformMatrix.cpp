#include <math.h>
#include <iostream>
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

bool closeEnough(const float& a, const float& b, const float& epsilon = std::numeric_limits<float>::epsilon()) {
    return (epsilon > fabs(a - b));
}

#define EULER_X 2
#define EULER_Y 1
#define EULER_Z 0

std::array<float,3> eulerAngles(const double4x4& R) {
    //check for gimbal lock
    if (closeEnough(R[2][0], -1.0f) || closeEnough(R[2][0], 1.0f)) {
        std::cout << "CASE 1" << std::endl;
        float x, y;
        float z = 0;
        float d = atan2(R[0][1], R[0][2]);

        if (closeEnough(R[2][0], -1.0f)) {
            y = M_PI/2;
            x = d;
        } else {
            y = -M_PI/2;
            x = -d;
        }

        return { x, y, z };
    } else { //two solutions exist
        float y1 = -asin(R[2][0]);
        float y2 = M_PI - y1;

        float x1 = atan2(R[2][1] / cos(y1), R[2][2] / cos(y1));
        float x2 = atan2(R[2][1] / cos(y2), R[2][2] / cos(y2));

        float z1 = atan2(R[1][0] / cos(y1), R[0][0] / cos(y1));
        float z2 = atan2(R[1][0] / cos(y2), R[0][0] / cos(y2));

        //choose one solution to return
        //for example the "shortest" rotation
        if ((fabs(x1) + fabs(y1) + fabs(z1)) <= (fabs(x2) + fabs(y2) + fabs(z2))) {
            return { x1, y1, z1 };
        } else {
            return { x2, y2, z2 };
        }
    }
}

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
                if (closeEnough(values[x][y], 0.0f)) {
                    str << "0 ";
                } else {
                    str << values[x][y] << " ";
                }
            }
            str << std::endl;
        }

        return str.str();
    }

    TransformMatrix TransformMatrix::rotationX(float alpha)
    {
        TransformMatrix m = TransformMatrix::identity();
        m.values[1][1] = cos(alpha);
        m.values[1][2] = -sin(alpha);
        m.values[2][1] = sin(alpha);
        m.values[2][2] = cos(alpha);
        return m;
    }
    TransformMatrix TransformMatrix::rotationY(float alpha)
    {
        TransformMatrix m = TransformMatrix::identity();
        m.values[0][0] = cos(alpha);
        m.values[0][2] = sin(alpha);
        m.values[2][0] = -sin(alpha);
        m.values[2][2] = cos(alpha);
        return m;
    }
    TransformMatrix TransformMatrix::rotationZ(float alpha)
    {
        TransformMatrix m = TransformMatrix::identity();
        m.values[0][0] = cos(alpha);
        m.values[0][1] = -sin(alpha);
        m.values[1][0] = sin(alpha);
        m.values[1][1] = cos(alpha);
        return m;
    }

    std::string TransformMatrix::toURDF()
    {
        auto rpy = eulerAngles(values);

        std::stringstream ss;
        ss << "<origin";
        ss << " xyz=\"" << values[0][3]/1000 << " " << values[1][3]/1000 << " " << values[2][3]/1000 << "\"";
        ss << " rpy=\"" << rpy[0] << " " << rpy[1] << " " << rpy[2] << "\"";
        ss << "/>";

        return ss.str();
    }
}
