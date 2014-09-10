#include <sstream>
#include <json/json.h>
#include "TransformMatrix.h"

namespace Metabot
{
    TransformMatrix::TransformMatrix()
    {
        values[3][3] = 1;
    }

    TransformMatrix TransformMatrix::zero()
    {
        TransformMatrix matrix;

        // Initializing to identity
        for (int x=0; x<4; x++) {
            for (int y=0; y<4; y++) {
                matrix.values[x][y] = 0;
            }
        }
    
        matrix.values[3][3] = 1;
        return matrix;
    }

    TransformMatrix TransformMatrix::identity()
    {
        TransformMatrix matrix;

        // Initializing to identity
        for (int x=0; x<4; x++) {
            for (int y=0; y<4; y++) {
                matrix.values[x][y] = (x==y);
            }
        }
        
        matrix.values[3][3] = 1;
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

    TransformMatrix TransformMatrix::multiply(TransformMatrix &other)
    {
        TransformMatrix result;

        for (int x=0; x<4; x++) {
            for (int y=0; y<4; y++) {
                float value = 0.0;
                for (int i=0; i<4; i++) {
                    value += values[x][i] * other.values[i][y];
                }
                result.values[x][y] = value;
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
                value += vector.values[y] * values[i][y];
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
                str << values[x][y] << " ";
            }
            str << std::endl;
        }

        return str.str();
    }
}
