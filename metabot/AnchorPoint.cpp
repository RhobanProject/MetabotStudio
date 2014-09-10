#include <iostream>
#include <sstream>
#include "AnchorPoint.h"
#include "Component.h"

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
    
    void AnchorPoint::attach(ComponentInstance *instance_)
    {
        Component *component = instance_->component;
        if (component->type != type) {
            std::stringstream ss;
            ss << "Can't attach component " << component->name << " of type " << component->type << " on an anchor " << type;
            throw ss.str();
        }
        instance_->compile();
        instance = instance_;
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
