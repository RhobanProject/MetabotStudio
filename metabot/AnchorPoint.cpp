#include <iostream>
#include <sstream>
#include "AnchorPoint.h"
#include "Component.h"

namespace Metabot
{
    AnchorPoint::AnchorPoint(Json::Value json, TransformMatrix matrix_)
        : type(""), matrix(matrix_), anchor(NULL), above(false), alpha(0.0)
    {
        if (json.isObject()) {
            type = json["type"].asString();
            male = json["male"].asBool();
            female = json["female"].asBool();
        }
    }
            
    AnchorPoint::~AnchorPoint()
    {
        if (anchor != NULL) {
            delete anchor;
        }
    }
    
    void AnchorPoint::attach(AnchorPoint *anchor_, bool above_)
    {
        above = above_;
        if (anchor_->type != type
            || !((male && anchor_->female) || (female && anchor_->male))) {
            std::stringstream ss;
            ss << "Can't attach anchor " << anchor_->type << " on an anchor " << type;
            throw ss.str();
        }
        anchor = anchor_;

        if (above) {
            anchor->attach(this, false);
        }
    }

    Model AnchorPoint::toModel()
    {
        Model m;

        if (above) {
            if (anchor != NULL) {
                m = anchor->toModel();
                m.rotateZ(alpha);
                m.apply(matrix);
            }
        } else {
            if (instance != NULL) {
                m = instance->toModel();
                m.apply(matrix.invert());
            }
        }

        return m;
    }
            
#ifdef OPENGL
    void AnchorPoint::openGLDraw()
    {
        if (above) {
            matrix.openGLMult();
            anchor->openGLDraw();
        } else {
            matrix.invert().openGLMult();
            instance->openGLDraw();
        }
    }
#endif
}
