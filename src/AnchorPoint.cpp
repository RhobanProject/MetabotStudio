#include <iostream>
#include "AnchorPoint.h"

namespace Metabot
{
    AnchorPoint::AnchorPoint(std::string type_, TransformMatrix matrix_)
        : type(type_), matrix(matrix_), instance(NULL), alpha(0.0)
    {
    }

    AnchorPoint::~AnchorPoint()
    {
        if (instance != NULL) {
            delete instance;
        }
    }

    Model AnchorPoint::toModel()
    {
        Model m;

        if (instance != NULL) {
            m = instance->toModel();
        }

        m.rotateZ(alpha);

        return m;
    }
}
