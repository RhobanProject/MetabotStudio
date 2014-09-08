#include "AnchorPoint.h"

namespace Metabot
{
    AnchorPoint::AnchorPoint(std::string type_, TransformMatrix matrix_)
        : type(type_), matrix(matrix_), component(NULL)
    {
    }
}
