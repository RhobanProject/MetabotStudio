#include <iostream>
#include "ModelRef.h"

namespace Metabot
{
    ModelRef::ModelRef(std::string name_, TransformMatrix matrix_)
        : name(name_), matrix(matrix_)
    {
    }
}
