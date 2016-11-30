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
#include <symbolicc++.h>
#include <LinearMath/btTransform.h>

bool closeEnough(const float& a, const float& b, const float& epsilon = std::numeric_limits<float>::epsilon()) {
    return (epsilon > fabs(a - b));
}

#define EULER_X 2
#define EULER_Y 1
#define EULER_Z 0

std::array<float,3> eulerAngles(const double4x4& R) {
    //check for gimbal lock
    if (closeEnough(R[2][0], -1.0f) || closeEnough(R[2][0], 1.0f)) {
        float x, y;
        float z = 0;

        if (closeEnough(R[2][0], -1.0f)) {
            float d = atan2(R[0][1], R[0][2]);
            y = M_PI/2;
            x = d;
        } else {
            float d = atan2(-R[0][1], -R[0][2]);
            y = -M_PI/2;
            x = d;
        }

        return { x, y, z };
    } else { //two solutions exist
        float y1 = -asin(R[2][0]);
        if (y1 != y1) {
            if (R[2][0] > 0) y1 = M_PI/2;
            else y1 = -M_PI/2;
        }
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
        inverse.values[3][3] = 1;

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
            
    Json::Value TransformMatrix::toJson()
    {
        Json::Value json(Json::arrayValue);

        for (int x=0; x<4; x++) {
            json[x] = Json::Value(Json::arrayValue);
            for (int y=0; y<4; y++) {
                json[x][y] = values[x][y];
            }
        }

        return json;
    }
    
    TransformMatrix TransformMatrix::fromJSON(Json::Value root)
    {
        TransformMatrix matrix;

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
            
    TransformMatrix TransformMatrix::fromJSON(std::string json)
    {
        Json::Value root;
        Json::Reader reader;

        if (!reader.parse(json, root)) {
            std::stringstream ss;
            ss << "Unable to decode matrix from: " << json;
            throw ss.str();
        }

        return fromJSON(root);
    }

    TransformMatrix TransformMatrix::multiply(TransformMatrix other)
    {
        TransformMatrix result;
        MATRIX_PRODUCT_4X4(result.values, values, other.values);
        result.values[3][3] = 1;

        return result;
    }
            
    Vect TransformMatrix::apply(Vect vector)
    {
        Vect output;
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
    TransformMatrix TransformMatrix::rotationZP(float alpha)
    {
        TransformMatrix m = TransformMatrix::identity();
        m.values[0][0] = -sin(alpha);
        m.values[0][1] = -cos(alpha);
        m.values[1][0] = cos(alpha);
        m.values[1][1] = -sin(alpha);
        return m;
    }
            
    Vect TransformMatrix::toRPY()
    {
        auto rpy = eulerAngles(values);

        return Vect(rpy[0], rpy[1], rpy[2]);
    }
    
    std::string TransformMatrix::toJS()
    {
        auto rpy = eulerAngles(values);

        std::stringstream ss;
        ss << "[" << values[0][3]/1000 << "," << values[1][3]/1000 << "," << values[2][3]/1000 << "], ";
        ss << "[" << rpy[0] << "," << rpy[1] << "," << rpy[2] << "]";
        
        return ss.str();
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

    std::string TransformMatrix::toSDF()
    {
        auto rpy = eulerAngles(values);
        
        std::stringstream ss;
        ss << "<pose>";
        ss << values[0][3]/1000 << " " << values[1][3]/1000 << " " << values[2][3]/1000 << " ";
        ss << rpy[0] << " " << rpy[1] << " " << rpy[2];
        ss << "</pose>";

        /*
        std::stringstream ss;
        ss << "<origin";
        ss << " xyz=\"" << values[0][3]/1000 << " " << values[1][3]/1000 << " " << values[2][3]/1000 << "\"";
        ss << " rpy=\"" << rpy[0] << " " << rpy[1] << " " << rpy[2] << "\"";
        ss << "/>";
        */

        return ss.str();
    }
            
    TransformMatrix TransformMatrix::translation(float x, float y, float z)
    {
        TransformMatrix m = TransformMatrix::identity();
        m.values[0][3] = x;
        m.values[1][3] = y;
        m.values[2][3] = z;

        return m;
    }
            
    float TransformMatrix::x()
    {
        return values[0][3];
    }

    float TransformMatrix::y()
    {
        return values[1][3];
    }

    float TransformMatrix::z()
    {
        return values[2][3];
    }
           
    void TransformMatrix::setX(float x)
    {
        values[0][3] = x;
    }

    void TransformMatrix::setY(float y)
    {
        values[1][3] = y;
    }

    void TransformMatrix::setZ(float z)
    {
        values[2][3] = z;
    }

    Symbolic TransformMatrix::toSymbolic()
    {
        Symbolic m("m", 4, 4);
        for (int i=0; i<4; i++)
        for (int j=0; j<4; j++) {
            m(i, j) = ((fabs(values[i][j]) > 1e-7) ? values[i][j] : 0);
        }

        return m;
    }
            
    btTransform TransformMatrix::toBullet()
    {
        return btTransform(
                btMatrix3x3(values[0][0], values[0][1], values[0][2],
                            values[1][0], values[1][1], values[1][2],
                            values[2][0], values[2][1], values[2][2]),
                btVector3(x()/1000.0, y()/1000.0, z()/1000.0)
                );
    }

    TransformMatrix TransformMatrix::fromBullet(btTransform trans)
    {
        auto matrix = TransformMatrix::identity();
        auto origin = trans.getOrigin();
        auto base = trans.getBasis();

        // Position
        matrix.values[0][3] = origin.getX()*1000.0;
        matrix.values[1][3] = origin.getY()*1000.0;
        matrix.values[2][3] = origin.getZ()*1000.0;

        // Rotation
        for (int x=0; x<3; x++) {
            for (int y=0; y<3; y++) {
                matrix.values[x][y] = base[x][y];
            }
        }

        return matrix;
    }
}
