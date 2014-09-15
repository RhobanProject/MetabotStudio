#include <math.h>
#include <iostream>
#include <sstream>
#include "Backend.h"
#include "AnchorPoint.h"
#include "Component.h"

namespace Metabot
{
    AnchorPoint::AnchorPoint(Json::Value json, TransformMatrix matrix_)
        : type(""), matrix(matrix_), anchor(NULL), above(true), alpha(0.0)
    {
        if (json.isObject()) {
            type = json["type"].asString();
            male = json["male"].asBool();
            female = json["female"].asBool();
        }

#ifdef OPENGL
        highlight = false;
#endif
    }

    AnchorPoint::~AnchorPoint()
    {
        if (anchor != NULL && above) {
            delete anchor;
        }
    }

    bool AnchorPoint::isCompatible(AnchorPoint *anchor)
    {
        if (anchor == NULL) {
            return type == "root";
        } else {
            if (type == anchor->type) {
                return (male && anchor->female)
                    || (female && anchor->male);
            }
        }

        return false;
    }

    void AnchorPoint::attach(AnchorPoint *anchor_, bool above_)
    {
        above = above_;

        if (!isCompatible(anchor_)) {
            anchor = NULL;
            /*
               std::stringstream ss;
               if (anchor_ == NULL) {
               ss << "Can't attach anchor (root) on an anchor " << type;
               } else {
               ss << "Can't attach anchor " << anchor_->type << " on an anchor " << type;
               }
               throw ss.str();
               */
            return;
        }
        anchor = anchor_;

        if (above) {
            anchor->attach(this, false);
        }
    }

    void AnchorPoint::detach(bool remove)
    {
        if (anchor != NULL) {
            if (remove) {
                delete anchor;
            }
            anchor = NULL;
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
        if (anchor != NULL) {
            if (above) {
                matrix.openGLMult();
                anchor->openGLDraw();
            } else {
                matrix.invert().openGLMult();
                instance->openGLDraw(anchor->highlight);
            }
        } else {
            if (above && highlight) {
                matrix.openGLMult();
                Model m = instance->component->backend->getModel("anchor");
                m.r = 0.4;
                m.g = 1.0;
                m.b = 0.3;
                m.openGLDraw();
            }
        }
    }
#endif
}
